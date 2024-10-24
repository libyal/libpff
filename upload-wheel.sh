#!/bin/bash

set -euo pipefail

wheel="$1"
url="s3://everlaw-build-artifacts/libpff/$(basename "$wheel")"

if aws s3 ls "$url" &>/dev/null; then
    >&2 echo "$url already exists!"
    exit 0
fi

aws s3 cp --only-show-errors "$wheel" "$url"
>&2 echo "Uploaded $url"
