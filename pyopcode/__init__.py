"""public interface"""

import os

name = 'pyopcode'
path = '_pyopcode.pyd'
try:
    import importlib.machinery
    loader = importlib.machinery.ExtensionFileLoader(name, path)
    pyopcode = loader.load_module()
except:
    import imp
    pyopcode = imp.load_dynamic(name, path)

Model = pyopcode.Model
Collision = pyopcode.Collision

__author__ = "Eelco Hoogendoorn"
__license__ = "LGPL"
__email__ = "eelco@clinicalgraphics.com"

pkg_dir = os.path.abspath(os.path.dirname(__file__))

__version__ = '0.1.5'