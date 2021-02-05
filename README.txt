--- PROJECT SUBMISSION NOTES ---

Catt Symonds
101209214
Game Engine Development II 
Assignment 1 Submission


I am using Microsoft's DirectXToolkit for this project as I wouldn't have been able to deliver by the deadline without it.
I thought this was a more appropriate solution than submitting something late and terrible, or not submitting anything at all!

--- ACTUAL README STUFF --- 

There is an executable file in this project called texconv. This is a texture converter that can convert .png files to .dds files.

Usage: 

run command: 
texconv <filename.png> -pmalpha -m 1 -f BC3_UNORM

---

Do not use hr == S_OK to check for success. Use SUCCEEDED(hr) instead.

--- SOME KNOWN PROBLEMS WITH THIS SETUP ---

Narrowing conversion error

Fix: just cast it to whatever it wants
m_fullscreenRect = { 0, 0, backBufferWidth, backBufferHeight };

becomes

m_fullscreenRect = { 0, 0, static_cast<LONG>(backBufferWidth), static_cast<LONG>(backBufferHeight) };

