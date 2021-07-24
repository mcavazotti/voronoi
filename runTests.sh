make
for file in testes/*
do
    echo
    echo "testing $file"
    ./voronoi < "$file" | python3 draw.py
    # ./voronoi < "$file" 
done