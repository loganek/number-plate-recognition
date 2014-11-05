execute_process(
  WORKING_DIRECTORY "${number-plate-recognition_SOURCE_DIR}/doc" 
  COMMAND pdflatex "${number-plate-recognition_SOURCE_DIR}/doc/main_document.tex"
)