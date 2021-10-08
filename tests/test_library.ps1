# Tests library functions and types.
#
# Version: 20200427

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$LibraryTests = "allocation_table attached_file_io_handle attachment column_definition compression data_array data_array_entry data_block deflate descriptors_index encryption error file_header folder free_map index index_node index_value io_handle io_handle2 index_tree item item_descriptor item_tree item_values local_descriptor_node local_descriptor_value local_descriptors local_descriptors_tree mapi_value message multi_value name_to_id_map_entry notify offsets_index record_entry record_set reference_descriptor table table_block_index table_header table_index_value value_type"
$LibraryTestsWithInput = "file support"
$OptionSets = ""

$InputGlob = "*"

Function GetTestExecutablesDirectory
{
	$TestExecutablesDirectory = ""

	ForEach (${VSDirectory} in "msvscpp vs2008 vs2010 vs2012 vs2013 vs2015 vs2017 vs2019" -split " ")
	{
		ForEach (${VSConfiguration} in "Release VSDebug" -split " ")
		{
			ForEach (${VSPlatform} in "Win32 x64" -split " ")
			{
				$TestExecutablesDirectory = "..\${VSDirectory}\${VSConfiguration}\${VSPlatform}"

				If (Test-Path ${TestExecutablesDirectory})
				{
					Return ${TestExecutablesDirectory}
				}
			}
			$TestExecutablesDirectory = "..\${VSDirectory}\${VSConfiguration}"

			If (Test-Path ${TestExecutablesDirectory})
			{
				Return ${TestExecutablesDirectory}
			}
		}
	}
	Return ${TestExecutablesDirectory}
}

Function ReadIgnoreList
{
	param( [string]$TestProfileDirectory )

	$IgnoreFile = "${TestProfileDirectory}\ignore"
	$IgnoreList = ""

	If (Test-Path -Path ${IgnoreFile} -PathType "Leaf")
	{
		$IgnoreList = Get-Content -Path ${IgnoreFile} | Where {$_ -notmatch '^#.*'}
	}
	Return $IgnoreList
}

Function RunTest
{
	param( [string]$TestType )

	$TestDescription = "Testing: ${TestName}"
	$TestExecutable = "${TestExecutablesDirectory}\pff_test_${TestName}.exe"

	If (-Not (Test-Path -Path ${TestExecutable} -PathType "Leaf"))
	{
		Write-Host "${TestDescription} (" -nonewline
		Write-Host "SKIP" -foreground Cyan -nonewline
		Write-Host ")"

		Return ${ExitIgnore}
	}
	$Output = Invoke-Expression ${TestExecutable}
	$Result = ${LastExitCode}

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	Write-Host "${TestDescription} (" -nonewline

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host "FAIL" -foreground Red -nonewline
	}
	Else
	{
		Write-Host "PASS" -foreground Green -nonewline
	}
	Write-Host ")"

	Return ${Result}
}

Function RunTestWithInput
{
	param( [string]$TestType )

	$TestDescription = "Testing: ${TestName}"
	$TestExecutable = "${TestExecutablesDirectory}\pff_test_${TestName}.exe"

	If (-Not (Test-Path -Path ${TestExecutable} -PathType "Leaf"))
	{
		Write-Host "${TestDescription} (" -nonewline
		Write-Host "SKIP" -foreground Cyan -nonewline
		Write-Host ")"

		Return ${ExitIgnore}
	}
	$TestProfileDirectory = "input\.libpff"

	If (-Not (Test-Path -Path ${TestProfileDirectory} -PathType "Container"))
	{
		New-Item -ItemType "directory" -Path ${TestProfileDirectory}
	}
	$IgnoreList = ReadIgnoreList ${TestProfileDirectory}

	$Result = ${ExitSuccess}

	ForEach ($TestSetInputDirectory in Get-ChildItem -Path "input" -Exclude ".*")
	{
		If (-Not (Test-Path -Path ${TestSetInputDirectory} -PathType "Container"))
		{
			Continue
		}
		If (${TestSetInputDirectory} -Contains ${IgnoreList})
		{
			Continue
		}
		$TestSetName = ${TestSetInputDirectory}.Name

		If (Test-Path -Path "${TestProfileDirectory}\${TestSetName}\files" -PathType "Leaf")
		{
			$InputFiles = Get-Content -Path "${TestProfileDirectory}\${TestSetName}\files" | Where {$_ -ne ""}
		}
		Else
		{
			$InputFiles = Get-ChildItem -Path ${TestSetInputDirectory} -Include ${InputGlob}
		}
		ForEach ($InputFile in ${InputFiles})
		{
			$TestedWithOptions = $False

			ForEach ($OptionSet in ${OptionSets} -split " ")
			{
				$InputFileName = ${InputFile}.Name
				$TestDataOptionFile = "${TestProfileDirectory}\${TestSetName}\${InputFileName}.${OptionSet}"

				If (-Not (Test-Path -Path "${TestDataOptionFile}" -PathType "Leaf"))
				{
					Continue
				}
				$InputOptions = Get-content -Path "${TestDataOptionFile}" -First 1

				$Output = Invoke-Expression "${TestExecutable} ${InputOptions} ${InputFile}"
				$Result = $LastExitCode

				If (${Result} -ne ${ExitSuccess})
				{
					Break
				}
				$TestedWithOptions = $True
			}
			If ((${Result} -eq ${ExitSuccess}) -And (-Not (${TestedWithOptions})))
			{
				$Output = Invoke-Expression "${TestExecutable} ${InputFile}"
				$Result = ${LastExitCode}
			}
			If (${Result} -ne ${ExitSuccess})
			{
				Break
			}
		}
		If (${Result} -ne ${ExitSuccess})
		{
			Break
		}
	}
	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	Write-Host "${TestDescription} (" -nonewline

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host "FAIL" -foreground Red -nonewline
	}
	Else
	{
		Write-Host "PASS" -foreground Green -nonewline
	}
	Write-Host ")"

	Return ${Result}
}

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Host "Missing test executables directory." -foreground Red

	Exit ${ExitFailure}
}

$Result = ${ExitIgnore}

Foreach (${TestName} in ${LibraryTests} -split " ")
{
	# Split will return an array of a single empty string when LibraryTests is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	$Result = RunTest ${TestName}

	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
	}
}

Foreach (${TestName} in ${LibraryTestsWithInput} -split " ")
{
	# Split will return an array of a single empty string when LibraryTestsWithInput is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	If (Test-Path -Path "input" -PathType "Container")
	{
		$Result = RunTestWithInput ${TestName}
	}
	Else
	{
		$Result = RunTest ${TestName}
	}
	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
	}
}

Exit ${Result}

