Feature: Program should echo unknown commands
	Scenario: wsh should echo one word per line for unknown commands
		Given a file named "test" with:
		"""
		This is still a test
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "test"
		Then the output should contain:
		"""
		This
		is
		still
		a
		test
		"""
 		And the stderr should not contain anything
		Then 5 points are awarded

