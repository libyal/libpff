# Script that synchronizes the local test data

Function Retry
{
	param(
		[scriptblock]$Command
	)

	$Attempt = 1
	$Sleep = 4

	while ($true)
	{
		try
		{
			& $Command
			return
		}
		catch
		{
			if ($Attempt -ge 5)
			{
				throw
			}
			Start-Sleep -Seconds $Sleep
			$Attempt++
			$Sleep = $Sleep * 2
		}
	}
}

$TestsInputDirectory = "tests\input"
$TestSet = "public"
$TestFiles = "outlook.pst"

If (-Not (Test-Path "${TestsInputDirectory}\.pffexport_recovered"))
{
	New-Item -Name "${TestsInputDirectory}\.pffexport_recovered" -ItemType "directory" | Out-Null
	Write-Output "-mrecovered" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.pffexport_recovered\options"
}

If (-Not (Test-Path ${TestsInputDirectory}))
{
	New-Item -Name ${TestsInputDirectory} -ItemType "directory" | Out-Null
}
If (-Not (Test-Path "${TestsInputDirectory}\${TestSet}"))
{
	New-Item -Name "${TestsInputDirectory}\${TestSet}" -ItemType "directory" | Out-Null
}
ForEach ($TestFile in ${TestFiles} -split " ")
{
	$UrlTestFile = [System.Uri]::EscapeDataString("${TestFile}")
	$Url = "https://raw.githubusercontent.com/libyal/testdata/refs/heads/main/pst/${UrlTestFile}"

	$ProgressPreference = 'SilentlyContinue'
	Retry { Invoke-WebRequest -Uri ${Url} -OutFile "${TestsInputDirectory}\${TestSet}\${TestFile}" }
}
