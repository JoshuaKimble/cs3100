Feature: Copy command is processed properly
	Scenario: copy command takes exactly two operands, not one or three
		Given a file named "copynothing" with:
		"""
		copy
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "copynothing"
		Then the output should contain: 
		"""
		Invalid argument count
		"""
		Given a file named "copythree" with:
		"""
		copy one two three
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "copythree"
		Then the output should contain: 
		"""
		Invalid argument count
		"""
		Then 5 points are awarded

	Scenario: copy command copies a file
		Given a file named "copyab" with:
		"""
		copy filea fileb
		exit
		"""
		Given a file named "filea" with:
		"""
		this is a file
		full of data
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "copyab"
		And the stderr should not contain anything
		And the file "filea" should be equal to file "fileb"
		Then 10 points are awarded

	Scenario: copy command copies a folder
		Given a directory named "folder/subfolder1/subsubfolder"
		And a file named "folder/file1" with:
		"""
		file1
		"""
		And a file named "folder/subfolder/subfile1" with:
		"""
		subfile1
		"""
		And a file named "copyfolder" with: 
		"""
		copy folder newfolder
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "copyfolder"
		Then the stderr should not contain anything
		And the file "folder/file1" should be equal to file "newfolder/file1"
		And the file "folder/subfolder/subfile1" should be equal to file "newfolder/subfolder/subfile1"
		Then 20 points are awarded

	Scenario: copy command fails when copying a file onto itself
		Given a file named "copyxx" with:
		"""
		copy filex filex
		exit
		"""
		And a file named "filex" with:
		"""
		filex
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "copyxx"
		And the file "filex" should be equal to file "filex"
		Then 10 points are awarded

	Scenario: copy command fails when input file does not exist
		Given a file named "copynonexistentfile" with:
		"""
		copy thisfiledoesnotexist filec
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "copynonexistentfile"
		Then the file "filec" should not exist
		And the stderr should contain "No such file or directory"
		Then 5 points are awarded

	Scenario: copy command fails when input folder does not exist
		Given a file named "copynonexistentfolder" with:
		"""
		copy thisfolderdoesnotexist folderm 
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "copynonexistentfolder"
		Then the file "folderm" should not exist
		And the stderr should contain "No such file or directory"
		Then 5 points are awarded

