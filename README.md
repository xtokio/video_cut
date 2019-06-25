# Video cutter

## Requirements
FFMPEG is required to be installed in the system as is used in the background to cut the video file

## Compilation
```bash
g++ video_cut.cpp -std=c++11 -o video_cut
```

## Usage
Command ( video_cut ) follow by the video file and then the start second and the end second
```bash
./video_cut video_file.mp4 0 30
```