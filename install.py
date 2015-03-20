import subprocess
import shlex
import os
import sys

parentPath = '/'.join( sys.path[0].replace('\\', '/').split('/')[:-1] )
sys.path.append( parentPath )

import Forge


###############################################################################################
# Version
###############################################################################################


bcShadingMilestoneVersion = 0 # for announcing major milestones - may contain all of the below
bcShadingMajorVersion     = 0 # backwards-incompatible changes
bcShadingMinorVersion     = 0 # new backwards-compatible features
bcShadingPatchVersion     = 1 # bug fixes


###############################################################################################
# Environment
###############################################################################################


softwareEnvironment = 'f:/software/'
softwareName = 'bcShading_%s.%s.%s.%sdev' %( bcShadingMilestoneVersion, bcShadingMajorVersion, bcShadingMinorVersion, bcShadingPatchVersion )
softwarePath = '%s%s/' %( softwareEnvironment, softwareName )


###############################################################################################
# Folder creation
###############################################################################################


rslDir = 'rsl'
sdlDir = 'sdl'
shaderCount = 0

Forge.core.System().mkdir( '%s%s' %(softwarePath, sdlDir) )


###############################################################################################
# Shader compilation
###############################################################################################


print '>>> Compile Begin'

for file in os.listdir( rslDir ):
    if '.sl' in file:
		cmd = '"C:/Program Files/3Delight For Maya/bin/shaderdl-3dfm.exe" -d %s%s %s/%s' % ( softwarePath, sdlDir, rslDir, file )
		args = shlex.split(str(cmd))
		subprocess.Popen(args)
		shaderCount += 1
		print '>>>   %s."%s" is well compiled.' % ( str(shaderCount).zfill(3), file )

print '>>> Compile End'
