Feature: mkdir command is processed properly
	Scenario: mkdir command takes exactly one operand
		Given a file named "mkdirnothing" with:
		"""
		mkdir
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "mkdirnothing"
		Then the output should contain: 
		"""
		Invalid argument count
		"""
		Given a file named "mkdirtwo" with:
		"""
		mkdir one two
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "mkdirtwo"
		Then the output should contain: 
		"""
		Invalid argument count
		"""
		Then 5 points are awarded

	Scenario: mkdir command creates a folder
		Given a file named "mkdirfolder" with:
		"""
		mkdir mfoldera
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "mkdirfolder"
		And the stderr should not contain anything
		And a directory named "mfoldera" should exist
		Then 10 points are awarded

	Scenario: mkdir command fails if folder already exists
		Given a directory named "mfolderc"
		Given a file named "mkdirfolder" with:
		"""
		mkdir mfolderc
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "mkdirfolder"
		And the output should contain:
		"""
		File exists
		"""
		Then 5 points are awarded

