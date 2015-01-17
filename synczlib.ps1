# Script that synchronizes zlib
#
# Version: 20150117
#
#
function DownloadFile($Url, $Destination)
{
	$Client = New-Object Net.WebClient
	${Client}.DownloadFile(${Url}, ${Destination})
}

function ExtractZip($Zip, $Destination)
{
	$Shell = New-Object -ComObject Shell.Application
	$Archive = ${Shell}.NameSpace(${Zip})
	$Directory = ${Shell}.Namespace(${Destination})

	foreach($FileEntry in ${Archive}.items())
	{
		${Directory}.CopyHere(${FileEntry})
	}
}

$Filename = "${pwd}\zlib128.zip"

if (Test-Path ${Filename})
{
	Remove-Item -Path ${Filename} -Force
}
DownloadFile -Url "http://zlib.net/zlib128.zip" -Destination ${Filename}

if (Test-Path "zlib-1.2.8")
{
	Remove-Item -Path "zlib-1.2.8" -Force -Recurse
}
ExtractZip -Zip ${Filename} -Destination "${pwd}"

if (Test-Path "..\zlib")
{
	Remove-Item -Path "..\zlib" -Force -Recurse
}
Move-Item "zlib-1.2.8" "..\zlib"

