make
for file in saida/out*
do
    echo
    echo "$file"
    python3 draw_delaunay.py < $file
    # ./voronoi < "$file" 
done