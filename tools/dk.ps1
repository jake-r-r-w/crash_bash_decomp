# PowerShell twin of tools/dk for running commands by hand on Windows:
#   .\tools\dk.ps1 make -s
#   .\tools\dk.ps1 python3 tools/scripts/decomp.py status
$ErrorActionPreference = "Stop"
$Root = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
# The tag is the Dockerfile's git blob hash, like tools/dk: a Dockerfile change builds a new image.
$Hash = (git -C $Root hash-object tools/docker/Dockerfile).Substring(0, 12)
$Image = if ($env:PSX_DECOMP_IMAGE) { $env:PSX_DECOMP_IMAGE } else { "psx-decomp:$Hash" }

docker image inspect $Image *> $null
if ($LASTEXITCODE -ne 0) {
    Write-Host "Building toolchain image $Image (one-time, several minutes)..."
    docker build -q -t $Image (Join-Path $Root "tools\docker")
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}

# Pass the host git identity through so `decomp.py done --commit` can commit from the container.
$GitEnv = @()
$GN = git -C $Root config user.name 2>$null
$GE = git -C $Root config user.email 2>$null
if ($GN) { $GitEnv += @("-e", "GIT_AUTHOR_NAME=$GN", "-e", "GIT_COMMITTER_NAME=$GN") }
if ($GE) { $GitEnv += @("-e", "GIT_AUTHOR_EMAIL=$GE", "-e", "GIT_COMMITTER_EMAIL=$GE") }

docker run --rm -i @GitEnv -e HOME=/tmp -e PYTHONDONTWRITEBYTECODE=1 -v "${Root}:/work" -w /work $Image @args
exit $LASTEXITCODE
