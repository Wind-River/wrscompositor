python -c 'import wr_amb' 2>/dev/null
rm -rf output
mkdir output
find . -name '*.pyc' | xargs tar -cf tmp.tar
cd output
tar -xf ../tmp.tar
rm -f ../tmp.tar

cp ../wr-amb .
cp ../ipod/_*so ipod

# twisted dependencies
cp -f /usr/lib/python2.7/cgi.pyc .
cp ../zope-cortexa9hf-vfp zope

mkdir www
touch twisted/web/failure.xhtml
