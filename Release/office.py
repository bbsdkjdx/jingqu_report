#version 2.1
#wps文字：'kwps.application'
#wps表格：'ket.application'
#wps演示：'kwpp.application'

from win32com.client import DispatchEx as _dispatch
from os.path import isdir as _isdir
from os import walk as _walk
from os.path import join as _join

def enum_files(root,*allow_ext):
    allow_ext=list(map(lambda s:s.lower(),allow_ext))
    if len(allow_ext)==0:
        allow_ext=['']
    if not _isdir(root):
        return
    for di,fd,fl in _walk(root):
        for f in fl:
            for ext in allow_ext:
                if f.lower().endswith(ext):
                    yield _join(di,f)

class Excel(object):
    """An Excel application instance"""
    def __init__(self, visible):
        self.raw=_dispatch('Excel.Application')
        self.raw.Visible=visible

    def __del__(self):
        if self.raw.Visible:
            return
        for x in self.books:
            x.close()
        self.raw.Quit()

    @property
    def selection(self):
        return self.raw.Selection()
    
    @property
    def range(self,s):
        return self.raw.Range(s)

    @property
    def visible(self):
        return self.raw.Visible

    @visible.setter
    def visible(self,visible):
        self.raw.Visible=visible
    

    @property
    def books(self):
        return list(map(lambda x:self.Book(x,self),self.raw.Workbooks))

    def open(self,fn):
        _bk=self.raw.Workbooks.Open(fn)
        return self.Book(_bk,self)

    def new(self):
        _bk=self.raw.Workbooks.Add()
        return self.Book(_bk,self)

    class Book(object):
        """docstring for Book"""
        def __init__(self, _book,excel):
            self.raw=_book
            self._excel=excel

        def __del__(self):
            pass

        def __repr__(self):
            return super().__repr__()+'name="'+self.name+'"'

        @property
        def name(self):
            return self.raw.name
        

        @property
        def sheets(self):
            return list(map(lambda x:self.Sheet(x,self),self.raw.Sheets))

        def save(self):
            if self.raw==None:
                return
            self.raw.Save()

        def saveas(self,fn):
            if self.raw==None:
                return
            self.raw.SaveAs(fn)

        def close(self):
            if self.raw:
                self.raw.Close(0)
                self.raw=None
                
        class Sheet(object):
            """docstring for Sheet"""
            def __init__(self, sht,book):
                self.raw = sht
                self._book=book
            def __repr__(self):
                return '%s:name=%s.%s'%(super().__repr__(),self._book.name,self.name)

            @property
            def n_rows(self):
                return self.raw.UsedRange.Rows.Count

            @property
            def n_cols(self):
                return self.raw.UsedRange.Columns.Count

            def get_text(self,row,col):
                return self.raw.Cells(row,col).Value

            def set_text(self,row,col,txt):
                self.raw.Cells(row,col).Value=txt

            @property
            def name(self):
                return self.raw.Name

            @name.setter
            def name(self,nm):
                self.raw.Name=nm
            

            def print_out(self):
                self.raw.PrintOut()

            def copy_before(self,st=None):
                if st:
                    self.raw.Copy(st.raw)
                else:
                    self.raw.Copy()

            def move_before(self,st=None):
                if st:
                    self.raw.Move(st.raw)
                else:
                    self.raw.Move()

            def save_alone(self,fn):
                self.copy_before()
                self._book._excel.raw.ActiveWorkBook.SaveAs(fn)
                self._book._excel.raw.ActiveWorkBook.Close(0)

            def delete(self):
                self.raw.Delete()


class Word(object):
    """docstring for Word"""
    def __init__(self, visible):
        self.app=_dispatch('Word.Application')
        self.app.Visible=visible

    def __del__(self):
        for x in range(self.n_docs()):
            self.app.Documents(1).Close(0)
        self.app.Quit()
    @property
    def raw(self):
        return self.app
    
    @property
    def n_docs(self):
        return self.app.Documents.Count

    def get_doc(self,idx_1):
        return self.Doc(self.app.Documents(idx_1))
    
    @property
    def docs(self):
        return list(map(self.Doc,self.app.Documents))

    def open(self,fn):
        _dk=self.app.Documents.Open(fn)
        return self.Doc(_dk)

    def new(self):
        _dk=self.app.Documents.Add()
        return self.Doc(_dk)

    class Doc(object):
        """docstring for Doc"""
        def __init__(self, _doc):
            self.doc=_doc

        def __del__(self):
            pass

        @property
        def raw(self):
            return self.doc

        @property
        def shapes(self):
            return list(map(self.Shape,self.doc.Shapes))
        
        
        def get_text(self):
            return self.doc.Content.Text
        def get_shape(self,text):
            for sp in self.doc.Shapes:
                try:
                    txt=sp.TextFrame.TextRange.Text
                    if txt==text+'\r':
                        return self.Shape(sp)
                except:
                    continue
            raise Exception("can't find shape named:"+text)

        def get_table(self,idx_0):
            idx=0
            for ret in self.doc.Tables:#under some circumstances,Tables doesn't support indexing.
                if idx==idx_0:
                    break
                idx+=1
            return self.Table(ret)
        @property
        def tables(self):
            return [self.Table(x) for x in self.doc.Tables]

        def saveas(self,fn):
            self.doc.SaveAs(fn)

        @property
        def raw(self):
            return self.doc

        def close(self):
            self.doc.Close(0)

        class Shape(object):
            """docstring for Shape"""
            def __init__(self, _shp):
                self.shape=_shp
            @property
            def raw(self):
                return self.shape
            @property
            def text(self):
                return self.shape.TextFrame.TextRange.Text

            @text.setter
            def text(self,txt):
                self.shape.TextFrame.TextRange.Text=txt
            def adjust_spacing(self):
                n_line=self.shape.TextFrame.TextRange.ComputeStatistics(1)
                t_b=self.shape.TextFrame.MarginBottom+self.shape.TextFrame.MarginTop
                _pf=self.shape.TextFrame.TextRange.ParagraphFormat
                _pf.LineSpacingRule=4
                _pf.LineSpacing=(self.shape.Height-t_b)/n_line
                return _pf.LineSpacing

        class Table(object):
            def __init__(self,_tb):
                self.table=_tb
            @property
            def raw(self):
                return self.table
            def get_cell(self,row_1,col_1):
                return self.Cell(self.table.Cell(row_1,col_1))

            class Cell(object):
                """docstring for Cell"""
                def __init__(self, _cel):
                    self.cell = _cel
                @property
                def raw(self):
                    return self.cell
                @property
                def text(self):
                    return self.cell.Range.Text
                @text.setter
                def text(self,txt):
                    self.cell.Range.Text=txt
                def add_picture(self,pic_fn):
                    pic=self.cell.Range.InlineShapes.AddPicture(pic_fn)
                    sp=pic.ConvertToShape()
                    sp.Top=0
                    sp.Left=0
                    sp.Width=self.cell.Width-1
                    return sp
                    #sp.Height=self.cell.Height-1

