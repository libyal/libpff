# Tests C library functions and types.
#
# Version: 20170807

$ExitSuccess = 0
$ExitFailure = 1
$ExitIgnore = 77

$LibraryTests = "attached_file_io_handle column_definition data_array data_array_entry data_block descriptors_index error index index_node index_value io_handle item item_descriptor item_values local_descriptor_node local_descriptor_value local_descriptors multi_value name_to_id_map_entry notify offsets_index record_entry record_set reference_descriptor table table_block_index table_index_value"
$LibraryTestsWithInput = "file support"

$TestToolDirectory = "..\msvscpp\Release"

Function RunTest
{
	param( [string]$TestType )

	$TestDescription = "Testing: ${TestName}"
	$TestExecutable = "${TestToolDirectory}\pff_test_${TestName}.exe"

	$Output = Invoke-Expression ${TestExecutable}
	$Result = ${LastExitCode}

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	Write-Host "${TestDescription} " -nonewline

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host " (FAIL)"
	}
	Else
	{
		Write-Host " (PASS)"
	}
	Return ${Result}
}

If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\msvscpp\VSDebug"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2010\Release"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2010\VSDebug"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2012\Release"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2012\VSDebug"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2013\Release"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2013\VSDebug"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2015\Release"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	$TestToolDirectory = "..\vs2015\VSDebug"
}
If (-Not (Test-Path ${TestToolDirectory}))
{
	Write-Host "Missing test tool directory." -foreground Red

	Exit ${ExitFailure}
}

$Result = ${ExitIgnore}

Foreach (${TestName} in ${LibraryTests} -split " ")
{
	$Result = RunTest ${TestName}

	If (${Result} -ne ${ExitSuccess})
	{
		Break
	}
}

Foreach (${TestName} in ${LibraryTestsWithInput} -split " ")
{
	# TODO: add RunTestWithInput
	$Result = RunTest ${TestName}

	If (${Result} -ne ${ExitSuccess})
	{
		Break
	}
}

Exit ${Result}

