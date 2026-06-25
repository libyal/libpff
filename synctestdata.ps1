# Script that synchronizes the local test data

$TestsInputDirectory = "tests\input"
$TestSet = "public"
$TestFiles = "outlook.pst"

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
	Invoke-WebRequest -Uri ${Url} -OutFile "${TestsInputDirectory}\${TestSet}\${TestFile}"
}
