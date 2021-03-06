;; variable initiliazation
(defvar parsed nil) ;contains the assembled lines of code in a list
(defvar currline nil) ;string containing the line that is currently being assembled	
(defvar mnemonics '(("MOV" . "11111111") ("JNE" . "11111110") ("CMP" . "11111101") ("INC" . "11111100") ("L  " . "11111011") ("NOP" . "11111010"))) ;maps mnemonics to byte code
(defvar registers '(("ax" . "00000000") ("bx" . "00000001") ("cx" . "00000010") ("dx" . "00000011")))  
  
;writes out the assembled program into the file  
(defun writeout (fileName) 
	(with-open-file (stream (concatenate 'string fileName ".o") :direction :output) ;open the file
		(loop for line in parsed do (format stream line) (terpri stream)) ;for ea. line of code. write to file and newline
	)
)
  
;translate mnemonics into appopriate byte code  
;TODO: be less imperative, more functional
(defun parseLine (input) 
	;FIRST COLUMN
	(loop for cell in mnemonics do
		(if (string= (subseq input 0 3) (car cell)) ;check if mnemonic matches
			;(setq parsed (push (cdr cell) parsed))()))) ;if so add the appropriate byte code to "parsed"
			(setq currline (concatenate 'string (cdr cell) " "))())) ;if so add the appropriate byte code to "parsed"		
	;SECOND COLUMN
	(if (string= (subseq input 0 3) "JNE") 
		(setq currline (concatenate 'string currline (subseq input 4 (- (length input) 2))))
		(if (not (string= (subseq input 0 3) "NOP"))
		(loop for cell in registers do
			(if (string= (subseq input 4 6) (car cell)) 
				(setq currline (concatenate 'string currline (cdr cell))) #|(print (subseq input 4 6))|#))) )
	;THIRD COLUMN
	(if (or (string= (subseq input 0 3) "MOV") (string= (subseq input 0 3) "L  ") )
		(setq currline (concatenate 'string currline " " (subseq input 7 15))) ())
	(if (string= (subseq input 0 3) "CMP")
		(loop for cell in registers do
			(if (string= (subseq input 7 9) (car cell)) 
				(setq currline (concatenate 'string currline " " (cdr cell))) ())))
	(if (or (string= (subseq input 0 3) "JNE") (string= (subseq input 0 3) "INC")) 
		(setq currline (concatenate 'string currline " 11111111")) ())
	(if (string= (subseq input 0 3) "NOP") 
		(setq currline (concatenate 'string currline "11111111 11111111")) ())
	(setq parsed (push currline parsed))
	(setq currline nil)
) 

;reads the input file line by line and call the parse function			
(defun readFLine (fileName)
	(let ((in (open fileName :if-does-not-exist nil)))
	   (when in
		  (loop for line = (read-line in nil) ;get next line of file
		  while line do (parseLine line)) ;translate the line into binary
		  (close in) ;close stream
	   )
	)
)

;body of the program
(readFLine "testfile.sasm") ;read and assemble the input file
;(print parsed)
(ignore-errors (delete-file (probe-file (concatenate 'string "testfile.sasm" ".o")))) ;delete the old output file, if extant so it can be recreated
(setq parsed (reverse parsed))
(writeout "testfile.sasm") ;write the binary to an output file

;(print (cdr (assoc "mov" mnemonics :test #'equal)))