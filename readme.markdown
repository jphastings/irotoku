irotoku
=======

About
-----
_irotoku_ is a steganographic decoder, it will take a png image and read from it any hidden data written in a specific way.

The method used for hiding data is simple. Assuming we have chosen 1 bits (2 and 4 bits also work, but at the risk of being more noticeable), each pixel of colour is separated into R,G,B and the value representing the colour is modified so that when divided by 2, the remainder is the bit value we desire for our hidden text. (If we were using a 2-bit system, we would divide each value by 4 and the result would represent 2 bits of the data we're hiding).

This program goes through a file (using the _GD image library_) and returns the data hidden in your png!

Why Bother?
-----------
Many steganographic techniques require that you have the original file, this technique does not. The only indicator that there may be data hidden is that a noisy picture has been sent as a png (rather than a jpeg) and I'll hopefully have a jpeg compatible one created soon.

Examples
--------
Onces you've compiled it, try this:
	irotoku "test image.png" | less
And if you've feeling adventurous:
	irotoku -0 -b 2 "test image 2.png" | mpg123 -
If you do both at once, in different windows, there's a surprise :P
By the way, did I mention that [Jonathan Coulton](http://www.jonathancoulton.com/ "JC: Musician, Songwriter") is exceptionally cool?
I've used one of [my own photos](http://www.flickr.com/photos/jphastings/3005111956/ "Juggletastic") as the second test image.

How to Compile
--------------
Make sure you have the GD library installed on your computer, and run:
	gcc irotoku_decode.c -o irotoku -l gd
This will compile the decoder as 'irotoku' using the gd reference library. If you then want to make it a permanent addition to your computer's repertoire, you can do:
	sudo mv irotoku /usr/local/bin/
Or move _irotoku_ to any directory in your _path_ variable.

Encoding
--------
I haven't had the time to create a C encoding script, however I wrote a quick hack of one in PHP. Its up here as well, but you're on your own as to how to use it (for now).