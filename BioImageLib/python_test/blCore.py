# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.11
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_blCore', [dirname(__file__)])
        except ImportError:
            import _blCore
            return _blCore
        if fp is not None:
            try:
                _mod = imp.load_module('_blCore', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _blCore = swig_import_helper()
    del swig_import_helper
else:
    import _blCore
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


class blData(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, blData, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, blData, name)
    __repr__ = _swig_repr
    def __init__(self): 
        this = _blCore.new_blData()
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _blCore.delete_blData
    __del__ = lambda self : None;
    def name(self): return _blCore.blData_name(self)
blData_swigregister = _blCore.blData_swigregister
blData_swigregister(blData)

class blMatrix(blData):
    __swig_setmethods__ = {}
    for _s in [blData]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, blMatrix, name, value)
    __swig_getmethods__ = {}
    for _s in [blData]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, blMatrix, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _blCore.new_blMatrix(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _blCore.delete_blMatrix
    __del__ = lambda self : None;
    def numberOfRows(self): return _blCore.blMatrix_numberOfRows(self)
    def numberOfColumns(self): return _blCore.blMatrix_numberOfColumns(self)
    def __call__(self, *args): return _blCore.blMatrix___call__(self, *args)
    def __add__(self, *args): return _blCore.blMatrix___add__(self, *args)
    def __sub__(self, *args): return _blCore.blMatrix___sub__(self, *args)
    def __mul__(self, *args): return _blCore.blMatrix___mul__(self, *args)
    def __div__(self, *args): return _blCore.blMatrix___div__(self, *args)
    def __iadd__(self, *args): return _blCore.blMatrix___iadd__(self, *args)
    def __isub__(self, *args): return _blCore.blMatrix___isub__(self, *args)
    def __imul__(self, *args): return _blCore.blMatrix___imul__(self, *args)
    def __idiv__(self, *args): return _blCore.blMatrix___idiv__(self, *args)
    def __eq__(self, *args): return _blCore.blMatrix___eq__(self, *args)
    def __ne__(self, *args): return _blCore.blMatrix___ne__(self, *args)
    def name(self): return _blCore.blMatrix_name(self)
blMatrix_swigregister = _blCore.blMatrix_swigregister
blMatrix_swigregister(blMatrix)

class blImage(blData):
    __swig_setmethods__ = {}
    for _s in [blData]: __swig_setmethods__.update(getattr(_s,'__swig_setmethods__',{}))
    __setattr__ = lambda self, name, value: _swig_setattr(self, blImage, name, value)
    __swig_getmethods__ = {}
    for _s in [blData]: __swig_getmethods__.update(getattr(_s,'__swig_getmethods__',{}))
    __getattr__ = lambda self, name: _swig_getattr(self, blImage, name)
    __repr__ = _swig_repr
    def __init__(self, *args): 
        this = _blCore.new_blImage(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _blCore.delete_blImage
    __del__ = lambda self : None;
    def name(self): return _blCore.blImage_name(self)
    def load(self, *args): return _blCore.blImage_load(self, *args)
    def imageType(self): return _blCore.blImage_imageType(self)
    def imageSize(self): return _blCore.blImage_imageSize(self)
    def pixelType(self): return _blCore.blImage_pixelType(self)
    def dimension(self): return _blCore.blImage_dimension(self)
    def is2D(self): return _blCore.blImage_is2D(self)
    def is3D(self): return _blCore.blImage_is3D(self)
    def save(self, *args): return _blCore.blImage_save(self, *args)
    def saveInt2DImage(self, *args): return _blCore.blImage_saveInt2DImage(self, *args)
    def saveFloat2DImage(self, *args): return _blCore.blImage_saveFloat2DImage(self, *args)
    def saveInt3DImage(self, *args): return _blCore.blImage_saveInt3DImage(self, *args)
    def saveFloat3DImage(self, *args): return _blCore.blImage_saveFloat3DImage(self, *args)
    def saveIntColor2DImage(self, *args): return _blCore.blImage_saveIntColor2DImage(self, *args)
    def saveFloatColor2DImage(self, *args): return _blCore.blImage_saveFloatColor2DImage(self, *args)
    def saveIntColor3DImage(self, *args): return _blCore.blImage_saveIntColor3DImage(self, *args)
    def saveFloatColor3DImage(self, *args): return _blCore.blImage_saveFloatColor3DImage(self, *args)
    def itkInt2DImagePointer(self): return _blCore.blImage_itkInt2DImagePointer(self)
    def itkFloat2DImagePointer(self): return _blCore.blImage_itkFloat2DImagePointer(self)
    def itkInt3DImagePointer(self): return _blCore.blImage_itkInt3DImagePointer(self)
    def itkFloat3DImagePointer(self): return _blCore.blImage_itkFloat3DImagePointer(self)
    def itkIntColor2DImagePointer(self): return _blCore.blImage_itkIntColor2DImagePointer(self)
    def itkFloatColor2DImagePointer(self): return _blCore.blImage_itkFloatColor2DImagePointer(self)
    def itkIntColor3DImagePointer(self): return _blCore.blImage_itkIntColor3DImagePointer(self)
    def itkFloatColor3DImagePointer(self): return _blCore.blImage_itkFloatColor3DImagePointer(self)
    def setItkImage(self, *args): return _blCore.blImage_setItkImage(self, *args)
    def xSize(self): return _blCore.blImage_xSize(self)
    def ySize(self): return _blCore.blImage_ySize(self)
    def zSize(self): return _blCore.blImage_zSize(self)
    def setPixelInt(self, *args): return _blCore.blImage_setPixelInt(self, *args)
    def getPixelInt(self, *args): return _blCore.blImage_getPixelInt(self, *args)
blImage_swigregister = _blCore.blImage_swigregister
blImage_swigregister(blImage)
cvar = _blCore.cvar
blImage.TypeInt2D = _blCore.cvar.blImage_TypeInt2D
blImage.TypeFloat2D = _blCore.cvar.blImage_TypeFloat2D
blImage.TypeInt3D = _blCore.cvar.blImage_TypeInt3D
blImage.TypeFloat3D = _blCore.cvar.blImage_TypeFloat3D
blImage.TypeIntColor2D = _blCore.cvar.blImage_TypeIntColor2D
blImage.TypeFloatColor2D = _blCore.cvar.blImage_TypeFloatColor2D
blImage.TypeIntColor3D = _blCore.cvar.blImage_TypeIntColor3D
blImage.TypeFloatColor3D = _blCore.cvar.blImage_TypeFloatColor3D
blImage.TypeIntVector2D = _blCore.cvar.blImage_TypeIntVector2D
blImage.TypeFloatVector2D = _blCore.cvar.blImage_TypeFloatVector2D

# This file is compatible with both classic and new-style classes.


