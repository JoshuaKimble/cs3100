Feature: View command is processed properly
	Scenario: view command is recognized and accepts exactly one operand	
		Given a file named "viewnothing" with:

		"""
		view
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "viewnothing"
		Then the output should contain "Invalid argument count"
		Given a file named "viewtoomuch" with:

		"""
		view a b c
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "viewtoomuch"
		Then the output should contain "Invalid argument count"
 		And the stderr should not contain anything
		Then 5 points are awarded

	Scenario: view FILE displays contents of FILE on screen
		Given a file named "fourscore" with:
		"""
		Four score and seven years ago
		Our forefathers brought forth onto this
		continent a new nation
		"""
		Given a file named "viewfourscore" with:
		"""
		view fourscore
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "viewfourscore"
		Then the output should contain:
		"""
		Four score and seven years ago
		Our forefathers brought forth onto this
		continent a new nation
		"""
 		And the stderr should not contain anything
		Then 10 points are awarded

