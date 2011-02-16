
files=$*

for f in $files
do
	#just to create new frames
        if [ -f $f.jpg ]
        then
                continue
        fi

	echo $f.jpg
	./xbps -x0 -y0 -s1  $f > $f.ps
	convert $f.ps $f.jpg
done
	
