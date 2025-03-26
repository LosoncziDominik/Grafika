# Minimal makefile for Sphinx documentation
#

# You can set these variables from the command line, and also
# from the environment for the first two.
SPHINXOPTS    ?=
SPHINXBUILD   ?= sphinx-build
SOURCEDIR     = .
BUILDDIR      = _build

it:
	sphinx-build -M html . _build

open:
	xdg-open _build/html/index.html

upload:
	rsync -avz _build/html/* matip@ftp.uni-miskolc.hu:/home/gepesz/mat/matip/public_html/grafika
	echo 'http://www.uni-miskolc.hu/~matip/grafika'

check:
	xdg-open 'http://www.uni-miskolc.hu/~matip/grafika'

# Put it first so that "make" without argument is like "make help".
help:
	@$(SPHINXBUILD) -M help "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)

.PHONY: help Makefile

# Catch-all target: route all unknown targets to Sphinx using the new
# "make mode" option.  $(O) is meant as a shortcut for $(SPHINXOPTS).
%: Makefile
	@$(SPHINXBUILD) -M $@ "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS) $(O)
