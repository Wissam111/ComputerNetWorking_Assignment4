with open("1gb.txt", "wb") as out:
    out.seek(( 1024*1024 ) - 1)
    out.write(b'\0')