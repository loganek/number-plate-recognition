INCLUDE(../UseLATEX/UseLATEX.cmake)

add_latex_document(main_document.tex
  INPUTS chapter_introduction.tex
  chapter_basics.tex
  chapter_methods.tex
  chapter_ocr.tex
  chapter_plates.tex
  chapter_researches.tex
  chapter_results.tex
  chapter_summary.tex
  IMAGE_DIRS img
#  BIBFILES main_document.bib
  DEFAULT_PDF
)