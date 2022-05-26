import tkinter
import subprocess
import concurrent.futures

root = tkinter.Tk()
root.geometry("800x500+500+300")
root.title("ytmp3")

links = ""

def read():
    global links
    with open("yt.txt", "r") as file:
        cons = file.read()
    urls = cons.split("\n")
    for i in urls:
        if i:
            link = i.split("&")[0] +"\n"
            links += link
            lbl.configure(text=links)

def add():
    global links
    url = ent.get()
    ent.delete(0, "end")
    if url:
        link = url.split("&")[0] +"\n"
        links += link
        lbl.configure(text=links)

def download(url):
    if url:
        subprocess.Popen(f"C:\\Program Files\\Git\\git-bash.exe -c 'youtube-dl -x --audio-format mp3 {url}'")

def down():
    global links
    url = links.split("\n")

    with concurrent.futures.ThreadPoolExecutor() as executor:
        executor.map(download, url)

    quit()

ent = tkinter.Entry(root, width=80)
btn = tkinter.Button(root, text="Submit", command=add)
btn2 = tkinter.Button(root, text="Read from File", command=read)
lbl = tkinter.Label(root)
btn3 = tkinter.Button(root, text="Download", command=down)

ent.pack(pady=30)
btn.pack(pady=10)
btn2.pack(pady=10)
lbl.pack(pady=10)
btn3.pack(pady=10)

root.mainloop()
