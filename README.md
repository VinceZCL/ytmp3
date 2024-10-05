# ytmp3
auto youtube to mp3 downloader

## Usage  
Compile `ytdl.c` using any compiler  
Create a `yt.txt` folder and insert all desired youtube links  
Run `.exe` file with `yt.txt` in the same directory  
### yt.txt
```
https://www.youtube.com/watch?v=2rtMFs2v6Ys
https://www.youtube.com/watch?v=A89jD-Ifl9o&ab_channel=RuiCh.%E4%B8%89%E6%A3%AE%E3%83%AB%E3%82%A4

```  

## Notes  
Youtube links with AdBlock modification `&ab_channel=<CHANNEL_NAME>` is parsed, so it is usable  
If error occurs, you might have to change the path of `git-bash.exe` in `ytdl.c` and `ytdl.pyw`   

## Disclaimer  
Designed for Windows 

## Dependencies  
youtube-dl  
git bash  
