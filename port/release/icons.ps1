# Draws the port's own icon (no game art: an orange rounded square with "CB"), for what a download carries: Android's launcher icon at
# each of its sizes (port/android/res/mipmap-*/ic_launcher.png) and the Windows program's icon (port/platform/crashbash.ico, a PNG at each
# size). The window's icon while the game runs is the PS1's memory card icon instead, from the player's own data (game/main/saveicon.c).
#   powershell -ExecutionPolicy Bypass -File port/release/icons.ps1
Add-Type -AssemblyName System.Drawing
$port = Split-Path -Parent $PSScriptRoot

function Draw-Icon([int]$s) {
    $bmp = New-Object System.Drawing.Bitmap $s, $s, ([System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    $g.SmoothingMode = 'AntiAlias'
    $g.TextRenderingHint = 'AntiAliasGridFit'
    $g.Clear([System.Drawing.Color]::Transparent)
    $m = $s * 0.06; $w = $s - 2 * $m; $r = $s * 0.22
    $path = New-Object System.Drawing.Drawing2D.GraphicsPath
    $path.AddArc($m, $m, $r, $r, 180, 90)
    $path.AddArc($m + $w - $r, $m, $r, $r, 270, 90)
    $path.AddArc($m + $w - $r, $m + $w - $r, $r, $r, 0, 90)
    $path.AddArc($m, $m + $w - $r, $r, $r, 90, 90)
    $path.CloseFigure()
    $rect = New-Object System.Drawing.RectangleF $m, $m, $w, $w
    $brush = New-Object System.Drawing.Drawing2D.LinearGradientBrush $rect, ([System.Drawing.Color]::FromArgb(255, 255, 160, 40)), ([System.Drawing.Color]::FromArgb(255, 214, 80, 0)), 90
    $g.FillPath($brush, $path)
    $pen = New-Object System.Drawing.Pen ([System.Drawing.Color]::FromArgb(255, 120, 40, 0)), ([Math]::Max(1, $s * 0.03))
    $g.DrawPath($pen, $path)
    $font = New-Object System.Drawing.Font 'Arial Black', ($s * 0.34), ([System.Drawing.FontStyle]::Bold), ([System.Drawing.GraphicsUnit]::Pixel)
    $fmt = New-Object System.Drawing.StringFormat
    $fmt.Alignment = 'Center'; $fmt.LineAlignment = 'Center'
    $shadow = New-Object System.Drawing.RectangleF ($m + $s * 0.02), ($m + $s * 0.03), $w, $w
    $g.DrawString('CB', $font, (New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::FromArgb(140, 90, 30, 0))), $shadow, $fmt)
    $g.DrawString('CB', $font, [System.Drawing.Brushes]::White, $rect, $fmt)
    $g.Dispose()
    return $bmp
}

function Png-Bytes($bmp) {
    $ms = New-Object System.IO.MemoryStream
    $bmp.Save($ms, [System.Drawing.Imaging.ImageFormat]::Png)
    # the comma keeps PowerShell from unrolling the bytes one by one
    return ,$ms.ToArray()
}

# Android, at each density
$android = @{ 'mipmap-mdpi' = 48; 'mipmap-hdpi' = 72; 'mipmap-xhdpi' = 96; 'mipmap-xxhdpi' = 144; 'mipmap-xxxhdpi' = 192 }
foreach ($dir in $android.Keys) {
    $out = Join-Path $port "android\res\$dir"
    New-Item -ItemType Directory -Force $out | Out-Null
    $bmp = Draw-Icon $android[$dir]
    $bmp.Save((Join-Path $out 'ic_launcher.png'), [System.Drawing.Imaging.ImageFormat]::Png)
    $bmp.Dispose()
}

# An icon's frame as Windows' own bitmap: its header, the colours bottom row first (BGRA), and an all-clear mask, as alpha does the work
function Dib-Bytes($bmp) {
    $s = $bmp.Width
    $ms = New-Object System.IO.MemoryStream
    $w = New-Object System.IO.BinaryWriter $ms
    $w.Write([UInt32]40); $w.Write([Int32]$s); $w.Write([Int32]($s * 2)); $w.Write([UInt16]1); $w.Write([UInt16]32)
    $w.Write([UInt32]0); $w.Write([UInt32]0); $w.Write([Int32]0); $w.Write([Int32]0); $w.Write([UInt32]0); $w.Write([UInt32]0)
    $data = $bmp.LockBits((New-Object System.Drawing.Rectangle 0, 0, $s, $s), 'ReadOnly', 'Format32bppArgb')
    $row = New-Object byte[] ($s * 4)
    for ($y = $s - 1; $y -ge 0; $y--) {
        [System.Runtime.InteropServices.Marshal]::Copy([IntPtr]::Add($data.Scan0, $y * $data.Stride), $row, 0, $s * 4)
        $w.Write($row)
    }
    $bmp.UnlockBits($data)
    $w.Write((New-Object byte[] ([Math]::Floor(($s + 31) / 32) * 4 * $s)))
    $w.Flush()
    return ,$ms.ToArray()
}

# Windows: an .ico with a frame for each size it asks for: bitmaps, and a PNG at 256 as Windows has them
$sizes = @(16, 24, 32, 48, 64, 128, 256)
$images = New-Object 'System.Collections.Generic.List[byte[]]'
foreach ($s in $sizes) { $b = Draw-Icon $s; if ($s -ge 256) { $images.Add((Png-Bytes $b)) } else { $images.Add((Dib-Bytes $b)) }; $b.Dispose() }
$ico = New-Object System.IO.MemoryStream
$bw = New-Object System.IO.BinaryWriter $ico
$bw.Write([UInt16]0); $bw.Write([UInt16]1); $bw.Write([UInt16]$sizes.Count)
$offset = 6 + 16 * $sizes.Count
for ($i = 0; $i -lt $sizes.Count; $i++) {
    $s = $sizes[$i]
    $bw.Write([byte]($s % 256)); $bw.Write([byte]($s % 256)); $bw.Write([byte]0); $bw.Write([byte]0)
    $bw.Write([UInt16]1); $bw.Write([UInt16]32); $bw.Write([UInt32]$images[$i].Length); $bw.Write([UInt32]$offset)
    $offset += $images[$i].Length
}
foreach ($img in $images) { $bw.Write($img) }
$bw.Flush()
[System.IO.File]::WriteAllBytes((Join-Path $port 'platform\crashbash.ico'), $ico.ToArray())
"icons made: Android's in port/android/res, Windows' port/platform/crashbash.ico"
