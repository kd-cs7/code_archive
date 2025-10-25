param(
    [Parameter(Mandatory = $true)]
    [string]$f,       # Full path of the CSV file to be split
    [Parameter(Mandatory = $true)]
    [int]$n,          # Number of parts to split into
    [int]$h = 1       # Number of header lines (default: 1)
)

# Set the output directory to the same as the input file’s directory
$outputDir = Split-Path $f

# ===============================
# Get header lines
# ===============================
$header = Get-Content $f -TotalCount $h

# Get total line count
$totalLines = (Get-Content $f -ReadCount 0).Count
$dataLines  = $totalLines - $h
$linesPerFile = [math]::Ceiling($dataLines / $n)

# ===============================
# Split processing with StreamWriter
# ===============================
$part = 1
$lineCount = 0
$outFile = Join-Path $outputDir ("{0}_part{1}.csv" -f ([IO.Path]::GetFileNameWithoutExtension($f), $part))
$writer = New-Object System.IO.StreamWriter($outFile, $false, [System.Text.Encoding]::UTF8)

# Write header to the first file
foreach ($line in $header) { $writer.WriteLine($line) }

Get-Content $f -ReadCount 10000 | ForEach-Object {
    foreach ($line in $_) {
        # Skip header lines
        if ($header -contains $line) { continue }

        $writer.WriteLine($line)
        $lineCount++

        if ($lineCount -ge $linesPerFile -and $part -lt $n) {
            $writer.Close()
            $part++
            $lineCount = 0
            $outFile = Join-Path $outputDir ("{0}_part{1}.csv" -f ([IO.Path]::GetFileNameWithoutExtension($f), $part))
            $writer = New-Object System.IO.StreamWriter($outFile, $false, [System.Text.Encoding]::UTF8)
            # Write header lines to part2 and later files
            foreach ($line in $header) { $writer.WriteLine($line) }
        }
    }
}

$writer.Close()
Write-Host "Split complete. Output files: $part"
