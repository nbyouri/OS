PDF_PATH=	/usr/local/texlive/2014/bin/x86_64-darwin/

# If you want to update the bibliography file,
# run -> latex report.tex; bibtex report; latex report.tex
# and make again!

all:
	${PDF_PATH}/pdflatex report.tex && open report.pdf

clean:
	rm report.pdf report.dvi *.log *.out *.aux
