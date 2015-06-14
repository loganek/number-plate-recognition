INCLUDE(../UseLATEX/UseLATEX.cmake)

add_latex_document(main_document.tex
  INPUTS chapter_basics.tex
  chapter_methods.tex
  chapter_ocr.tex
  chapter_plates.tex
  chapter_researches.tex
  IMAGE_DIRS img
  BIBFILES main_document.bib
  DEFAULT_PDF
)