Feature: del command is processed properly
	Scenario: del command takes exactly one operands, not zero or two
		Given a file named "delnothing" with:
		"""
		del
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "delnothing"
		Then the output should contain: 
		"""
		Invalid argument count
		"""
		Given a file named "deltwo" with:
		"""
		del one two 
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "deltwo"
		Then the output should contain: 
		"""
		Invalid argument count
		"""
		Then 5 points are awarded

	Scenario: del command deletes a file
		Given a file named "delfile" with:
		"""
		del dfilea
		exit
		"""
		Given a file named "dfilea" with:
		"""
		this is a file
		full of data
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "delfile"
		And the stderr should not contain anything
		And a file named "dfilea" should not exist
		Then 10 points are awarded

