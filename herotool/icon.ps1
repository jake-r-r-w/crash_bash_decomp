# Draws the hero tool's own icon (no game art), as port/release/icons.ps1 draws the game's: the same rounded square, in the tool's
# periwinkle (gui.cpp's PERI, darkening past PERI_DEEP), with a hero's figure in white and a coral spark beside its head, for making one. It's
# the program's icon in Explorer and the taskbar (herotool/src/herotool.ico, a frame at each size; herotool.rc), and so its window's.
#   powershell -ExecutionPolicy Bypass -File herotool/icon.ps1 [-Preview FOLDER]
param([string]$Preview = "")
Add-Type -AssemblyName System.Drawing
$tool = $PSScriptRoot

function Rounded([float]$x, [float]$y, [float]$w, [float]$h, [float]$r) {
    $p = New-Object System.Drawing.Drawing2D.GraphicsPath
    $p.AddArc($x, $y, $r, $r, 180, 90)
    $p.AddArc($x + $w - $r, $y, $r, $r, 270, 90)
    $p.AddArc($x + $w - $r, $y + $h - $r, $r, $r, 0, 90)
    $p.AddArc($x, $y + $h - $r, $r, $r, 90, 90)
    $p.CloseFigure()
    return $p
}

# A four-pointed spark at (cx, cy), r from its middle to a point
function Spark($g, [float]$cx, [float]$cy, [float]$r, $brush) {
    $k = $r * 0.28
    $pts = @(
        (New-Object System.Drawing.PointF $cx, ($cy - $r)), (New-Object System.Drawing.PointF ($cx + $k), ($cy - $k)),
        (New-Object System.Drawing.PointF ($cx + $r), $cy), (New-Object System.Drawing.PointF ($cx + $k), ($cy + $k)),
        (New-Object System.Drawing.PointF $cx, ($cy + $r)), (New-Object System.Drawing.PointF ($cx - $k), ($cy + $k)),
        (New-Object System.Drawing.PointF ($cx - $r), $cy), (New-Object System.Drawing.PointF ($cx - $k), ($cy - $k)))
    $g.FillPolygon($brush, [System.Drawing.PointF[]]$pts)
}

function Draw-Icon([int]$s) {
    $bmp = New-Object System.Drawing.Bitmap $s, $s, ([System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    $g.SmoothingMode = 'AntiAlias'
    $g.Clear([System.Drawing.Color]::Transparent)
    # the square, as the game's
    $m = $s * 0.06; $w = $s - 2 * $m; $r = $s * 0.22
    $path = Rounded $m $m $w $w $r
    $rect = New-Object System.Drawing.RectangleF $m, $m, $w, $w
    $brush = New-Object System.Drawing.Drawing2D.LinearGradientBrush $rect, ([System.Drawing.Color]::FromArgb(255, 0x9F, 0xAF, 0xF0)), ([System.Drawing.Color]::FromArgb(255, 0x5A, 0x6A, 0xB4)), 90
    $g.FillPath($brush, $path)
    $pen = New-Object System.Drawing.Pen ([System.Drawing.Color]::FromArgb(255, 0x2C, 0x35, 0x66)), ([Math]::Max(1, $s * 0.03))
    $g.DrawPath($pen, $path)
    # the hero: a head and shoulders, white, with a soft shadow under them
    $white = New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::White)
    $shade = New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::FromArgb(110, 0x16, 0x1B, 0x33))
    $cx = $s * 0.44; $hr = $s * 0.13; $hy = $s * 0.38
    $bw = $s * 0.50; $bt = $s * 0.56; $bb = $s * 0.86
    foreach ($pass in 0, 1) {
        $d = if ($pass -eq 0) { $s * 0.025 } else { 0 }
        $b = if ($pass -eq 0) { $shade } else { $white }
        $g.FillEllipse($b, ($cx - $hr + $d), ($hy - $hr + $d * 1.4), (2 * $hr), (2 * $hr))
        $body = Rounded ($cx - $bw / 2 + $d) ($bt + $d * 1.4) $bw (($bb - $bt) * 1.6) ($bw * 0.9)
        $g.SetClip((New-Object System.Drawing.RectangleF $m, $m, $w, ($bb - $m)))
        $g.FillPath($b, $body)
        $g.ResetClip()
    }
    # the spark, coral (gui.cpp's CORAL) with a dark edge so it shows on the light top
    $coral = New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::FromArgb(255, 0xF5, 0x7E, 0x63))
    $edge = New-Object System.Drawing.SolidBrush ([System.Drawing.Color]::FromArgb(255, 0x2C, 0x35, 0x66))
    $sx = $s * 0.71; $sy = $s * 0.29; $sr = $s * 0.15
    Spark $g $sx $sy ($sr + [Math]::Max(1, $s * 0.025)) $edge
    Spark $g $sx $sy $sr $coral
    $g.Dispose()
    return $bmp
}

function Png-Bytes($bmp) {
    $ms = New-Object System.IO.MemoryStream
    $bmp.Save($ms, [System.Drawing.Imaging.ImageFormat]::Png)
    return ,$ms.ToArray()
}

# An icon's frame as Windows' own bitmap (as icons.ps1's): its header, the colours bottom row first (BGRA), and an all-clear mask
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

$sizes = @(16, 24, 32, 48, 64, 128, 256)
if ($Preview -ne "") {
    New-Item -ItemType Directory -Force $Preview | Out-Null
    foreach ($s in $sizes) { $b = Draw-Icon $s; $b.Save((Join-Path $Preview "herotool_$s.png"), [System.Drawing.Imaging.ImageFormat]::Png); $b.Dispose() }
    "previews in $Preview"
    return
}
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
[System.IO.File]::WriteAllBytes((Join-Path $tool 'src\herotool.ico'), $ico.ToArray())
"icon made: herotool/src/herotool.ico"
