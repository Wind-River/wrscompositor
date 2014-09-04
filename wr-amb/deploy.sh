python -c 'import wr_amb' 2>/dev/null
rm -rf output
mkdir output
find . -name '*.pyc' | xargs tar -cf tmp.tar
cd output
tar -xf ../tmp.tar

cp ../wr-amb .
cp ../ipod/_*so ipod
mkdir www
