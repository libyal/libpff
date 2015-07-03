# Script that synchronizes zlib.
#
# Version: 20150118

function DownloadFile($Url, $Destination)
{
	$Client = New-Object Net.WebClient
	${Client}.DownloadFile(${Url}, ${Destination})
}

function ExtractZip($Filename)
{
	# AppVeyor does not seem to support extraction using "native ZIP" so we use 7z instead.
	$SevenZip = "C:\Program Files\7-Zip\7z.exe"

	if (Test-Path ${SevenZip})
	{
		# PowerShell will raise NativeCommandError if 7z writes to stdout or stderr
		# therefore 2>&1 is added and the output is stored in a variable.
		# The leading & and single quotes are necessary to compensate for the spaces in the path.
		$Output = Invoke-Expression -Command "& '${SevenZip}' -y x ${Filename} 2>&1"
	}
	else
	{
		$Shell = New-Object -ComObject Shell.Application
		$Archive = ${Shell}.NameSpace(${Filename})
		$Directory = ${Shell}.Namespace("${pwd}")

		foreach($FileEntry in ${Archive}.items())
		{
			${Directory}.CopyHere(${FileEntry})
		}
	}
}

$Filename = "${pwd}\zlib128.zip"
$Url = "http://zlib.net/zlib128.zip"
$ExtractedPath = "zlib-1.2.8"
$DestinationPath = "..\zlib"

if (Test-Path ${Filename})
{
	Remove-Item -Path ${Filename} -Force
}
DownloadFile -Url ${Url} -Destination ${Filename}

if (Test-Path ${ExtractedPath})
{
	Remove-Item -Path ${ExtractedPath} -Force -Recurse
}
ExtractZip -Filename ${Filename}

if (Test-Path ${DestinationPath})
{
	Remove-Item -Path ${DestinationPath} -Force -Recurse
}
Move-Item ${ExtractedPath} ${DestinationPath}

