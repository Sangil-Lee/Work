import sys

###############################################################################
# Declare system configuration: number of outputs, number of nodes per link
outputCount = 16
nodesPerLink = [1, 2, 0, 0, 0, 0, 0, 0]

###############################################################################
# No user-servicable parts beyond this point!


def createDisplay(width, height):
    print '4 0 1'
    print 'beginScreenProperties'
    print 'major 4'
    print 'minor 0'
    print 'release 1'
    print 'x 30'
    print 'y 10'
    print 'w %d' % (width)
    print 'h %d' % (height)
    print 'font "helvetica-medium-r-18.0"'
    print 'ctlFont "helvetica-medium-r-12.0"'
    print 'btnFont "helvetica-bold-r-14.0"'
    print 'fgColor index 0'
    print 'bgColor index 3'
    print 'textColor index 0'
    print 'ctlFgColor1 index 1'
    print 'ctlFgColor2 index 7'
    print 'ctlBgColor1 index 19'
    print 'ctlBgColor2 index 21'
    print 'topShadowColor index 2'
    print 'botShadowColor index 0'
    print 'endScreenProperties'
    print ''
    print '# (Static Text)'
    print 'object activeXTextClass'
    print 'beginObjectProperties'
    print 'major 4'
    print 'minor 1'
    print 'release 1'
    print 'x 0'
    print 'y 2'
    print 'w %d' % (width)
    print 'h 21'
    print 'font "helvetica-medium-r-18.0"'
    print 'fontAlign "center"'
    print 'fgColor index 14'
    print 'bgColor index 19'
    print 'useDisplayBg'
    print 'value {'
    print '  "$(P)$(R)"'
    print '}'
    print 'endObjectProperties'
    print ''

def createBit(link, node, bit, xPos, yPos, isOutput):
    print '# (Static Text)'
    print 'object activeXTextClass'
    print 'beginObjectProperties'
    print 'major 4'
    print 'minor 1'
    print 'release 1'
    print 'x %d' % (xPos)
    print 'y %d' % (yPos)
    print 'w 28'
    print 'h 14'
    print 'font "helvetica-medium-r-12.0"'
    print 'fontAlign "center"'
    print 'fgColor index 14'
    print 'bgColor index 19'
    print 'useDisplayBg'
    if (isOutput):
        print 'visPv "$(P)$(R)Mitigate%02d"' %(bit)
    else:
        print 'visPv "$(P)$(R)L%dN%dI%02d"' %(link, node, bit)
    print 'visMin "1"'
    print 'visMax "2"'
    print 'value {'
    if (isOutput):
        print '  "%02d"' % (bit)
    else:
        print '  "%04d"' % ((((node * 8) + link) * 64) + bit)
    print '}'
    print 'endObjectProperties'
    print ''

topMargin = 23
bottomMargin = 10
xMargin = 3
colWidth = 34
rowHeight = 14
colCount = 1
for l in range(0,len(nodesPerLink)):
    colCount += nodesPerLink[l]

createDisplay(2*xMargin+colWidth*colCount, topMargin+64*rowHeight+bottomMargin)
xPos = xMargin
for l in range(0,len(nodesPerLink)):
    for n in range(0,nodesPerLink[l]):
        yPos = topMargin + 63*rowHeight
        for b in range(0,64):
            createBit(l, n, b, xPos, yPos, False)
            yPos -= rowHeight
        xPos += colWidth
yPos = topMargin + 63*rowHeight
for b in range(0,outputCount):
    createBit(0, 0, b, xPos, yPos, True)
    yPos -= rowHeight
