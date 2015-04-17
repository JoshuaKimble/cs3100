@announce-stderr
Feature: Copy command is processed properly

	Scenario: copy command is recognized and echoed all on one line
		Given a file named "copyab" with:
		"""
		copy filea fileb
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "copyab"
		Then the output should contain:
		"""
		copy filea fileb
		"""
 		And the stderr should not contain anything
		Then 10 points are awarded

	Scenario: "." is replaced with PWD/sourcefile for copy command
		Given a file named "copyadot" with:
		"""
		copy filea .
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "copyadot"
		And dot is replaced with PWD/filea
 		And the stderr should not contain anything
		Then 20 points are awarded

