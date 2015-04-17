@announce-stderr
Feature: Program should echo unknown commands

	Scenario: wsh should echo one word per line for unknown commands
		Given a file named "test" with:
		"""
		This is a test
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "test"
		Then the output should contain:
		"""
		This
		is
		a
		test
		"""
 		And the stderr should not contain anything
		Then 30 points are awarded

