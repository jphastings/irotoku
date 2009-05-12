#!/Applications/MAMP/bin/php5/bin/php
<?php
$n = 2; // Number of bits per colour value
$n = pow(2,$n);

$file = new kedakaiFile("/Users/jp/Downloads/Images/06 Set It Off 2.mp3");

//$image = imagecreatefromjpeg("awesome.jpg");
$image = imagecreatefrompng("/Users/jp/Downloads/Images/rose.png");
$w = imagesx($image);
$h = imagesy($image);
$pc = $file->size()/floor(($w*$h*3*$n)/8);
if ($pc > 1)
	exit("There isn't enough room for that much data.\n Maximum is ".floor(($w*$h*3*$n)/8)." Bytes");
echo "We'll be using ".round($pc*100,2)."% of the available room in this image (".floor(($w*$h*3*$n)/(8*1024))."kB)\n";

$alldone = FALSE;

for ($x=0;$x<$w;++$x) {
	for ($y=0;$y<$h;++$y) {
		$rgb = imagecolorat($image,$x,$y);
		$rgb = array(($rgb >> 16) & 0xFF,($rgb >> 8) & 0xFF, $rgb & 0xFF);
		
		foreach ($rgb as $k=>$colour) {
			$bits = $file->nextBits($n);
			
			$diff = bindec($bits) - ($colour % pow(2,$n));
			if ($diff != 0) {
				$rgb[$k] += $diff;
				if ($rgb[$k] > 255)
					$rgb[$k] -= pow(2,$n);
				if ($rgb[$k] < 0)
					$rgb[$k] += pow(2,$n);
			}
		}
		$colourindex = imagecolorexact($image,$rgb[0],$rgb[1],$rgb[2]);
		$index = ($colourindex == -1) ? imagecolorallocate($image,$rgb[0],$rgb[1],$rgb[2]) : $colourindex;
			
		imagesetpixel($image,$x,$y,$colourindex);
		if ($alldone and count($file->buffer) == 0)
			break;
	}
	echo "Done Row $x\n";
	if ($alldone and count($file->buffer) == 0)
		break;
}

echo "\nAll good, data encoded - now writing PNG\n";
imagepng($image,"skull.png",9);
echo "Written to out.png\n";

class kedakaiFile {
	function __construct($filename) {
		$this->filesize = filesize($filename);
		$this->buffer = array();
		$this->fp = fopen($filename,"r");
	}
	
	function nextBits($n) {
		global $alldone;
		if (count($this->buffer) < $n) {
			$data = fread($this->fp,1);
			if ($data == NULL) {
				if (!$alldone)
					$this->buffer = array("0","0","0","0","0","0","0","0");
				$alldone = TRUE;
			} else {
				$chars = preg_split('//', $data, -1, PREG_SPLIT_NO_EMPTY);
				foreach($chars as $k=>$char) {
					$this->buffer = array_merge($this->buffer,preg_split('//', str_pad(decbin(ord($char)),8,0,STR_PAD_LEFT), -1, PREG_SPLIT_NO_EMPTY));
				}
			}
		}
		$bits = "";
		for ($i=0;$i<$n;++$i)
			$bits .= array_shift($this->buffer);
		return $bits;
	}

	function size() {
		return $this->filesize;
	}
}