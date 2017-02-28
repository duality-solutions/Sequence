from distutils.core import setup
setup(name='sequencespendfrom',
      version='1.0',
      description='Command-line utility for sequence "coin control"',
      author='Gavin Andresen',
      author_email='gavin@sequencefoundation.org',
      requires=['jsonrpc'],
      scripts=['spendfrom.py'],
      )
