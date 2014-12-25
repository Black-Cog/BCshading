import subprocess, shlex, os

rslDir = 'rsl'
sdlDir = 'sdl'
shaderCount = 0

print '>>> Compile Begin'

for file in os.listdir( rslDir ):
    if '.sl' in file:
		cmd = '"C:/Program Files/3Delight For Maya/bin/shaderdl-3dfm.exe" -d %s %s/%s' % ( sdlDir, rslDir, file )
		args = shlex.split(str(cmd))
		subprocess.Popen(args)
		shaderCount += 1
		print '>>>   %s."%s" is well compiled.' % ( str(shaderCount).zfill(3), file )

print '>>> Compile End'