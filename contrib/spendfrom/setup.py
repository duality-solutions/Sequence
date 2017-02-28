from distutils.core import setup
setup(name='silkspendfrom',
      version='1.0',
      description='Command-line utility for silk "coin control"',
      author='Gavin Andresen',
      author_email='gavin@silkfoundation.org',
      requires=['jsonrpc'],
      scripts=['spendfrom.py'],
      )
