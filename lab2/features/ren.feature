Feature: ren command is processed properly
	Scenario: ren command takes exactly two operands
		Given a file named "rennothing" with:
		"""
		ren
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "rennothing"
		Then the output should contain: 
		"""
		Invalid argument count
		"""
		Given a file named "renthree" with:
		"""
		ren one two three
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "renthree"
		Then the output should contain: 
		"""
		Invalid argument count
		"""
		Then 5 points are awarded

	Scenario: ren command renames a file
		Given a file named "renfileatofileb" with:
		"""
		ren rfilea rfileb
		exit
		"""
		Given a file named "rfilea" with:
		"""
		this is a file named rfilea
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "renfileatofileb"
		And the stderr should not contain anything
		And a file named "rfilea" should not exist
		And a file named "rfileb" should exist
		And the file "rfileb" should contain:
		"""
		this is a file named rfilea
		"""
		Then 10 points are awarded

	Scenario: ren command fails if input file does not exist
		Given a file named "renfileatofileb" with:
		"""
		ren nonexistantfilea rfilex
		exit
		"""
		When I run `../bin/wsh` interactively
		And I pipe in the file "renfileatofileb"
		And the output should contain:
		"""
		No such file or directory
		"""
		And a file named "rfilex" should not exist
		Then 5 points are awarded

