# Puts PNGs on one contact sheet: sheet.ps1 OUT.png COLS FILES...
param([string]$out, [int]$cols, [Parameter(ValueFromRemainingArguments = $true)][string[]]$files)
Add-Type -AssemblyName System.Drawing
$w = 320; $h = 240
$rows = [Math]::Ceiling($files.Count / $cols)
$bmp = New-Object System.Drawing.Bitmap ($cols * $w), ($rows * $h)
$g = [System.Drawing.Graphics]::FromImage($bmp)
$font = New-Object System.Drawing.Font 'Arial', 11
$i = 0
foreach ($f in $files) {
    if (Test-Path $f) {
        $img = [System.Drawing.Image]::FromFile((Resolve-Path $f))
        $x = ($i % $cols) * $w; $y = [Math]::Floor($i / $cols) * $h
        $g.DrawImage($img, $x, $y, $w, $h)
        $g.DrawString([IO.Path]::GetFileNameWithoutExtension($f), $font, [System.Drawing.Brushes]::Yellow, $x + 2, $y + 2)
        $img.Dispose()
    }
    $i++
}
$bmp.Save((Join-Path (Get-Location) $out), [System.Drawing.Imaging.ImageFormat]::Png)
$g.Dispose(); $bmp.Dispose()
