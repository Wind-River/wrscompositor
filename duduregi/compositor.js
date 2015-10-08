var windowList = null;
var indexes = null;

function relayout() {
    if (windowList === null || windowList.length == 0)
        return;

    var dim = Math.ceil(Math.sqrt(windowList.length));

    var cols = dim;
    var rows = Math.ceil(windowList.length / cols);

    var w = root.width / dim;
    var h = root.height / rows;

    var i;
    var ix = 0;
    var iy = 0;
    var lastDim = 1;

    indexes = new Array(dim * dim);

    for (i = 0; i < windowList.length; ++i) {
        if (i > 0) {
            var currentDim = Math.ceil(Math.sqrt(i + 1));
            if (currentDim == lastDim) {
                if (iy < currentDim - 1) {
                    ++iy;
                    if (iy == currentDim - 1)
                        ix = 0;
                } else {
                    ++ix;
                }
            } else {
                iy = 0;
                ix = currentDim - 1;
            }
            lastDim = currentDim;
        }

        indexes[iy * dim + ix] = i;
        windowList[i].index = iy * dim + ix;

        var cx = (ix + 0.5) * w;
        var cy = (iy + 0.5) * h;

        //windowList[i].targetScale = 0.98 * Math.min(w / windowList[i].width, h / windowList[i].height);

        //windowList[i].targetX = (cx - windowList[i].width / 2);
        //windowList[i].targetY = (cy - windowList[i].height / 2);
    }
}

function addWindow(window)
{
    if (windowList == null)
        windowList = new Array(0);

    windowList.push(window);
    relayout();
}

function removeWindow(window)
{
    var i;
    for (i = 0; i < windowList.length; ++i) {
        if (windowList[i] == window)
            break;
    }

    var index = windowList[i].index;
    var dim = Math.ceil(Math.sqrt(windowList.length));
    var maxY = Math.floor((windowList.length-1) / dim);

    var shrinking = Math.ceil(Math.sqrt(windowList.length - 1)) != dim;

    while (true) {
        var ix = index % dim;
        var iy = Math.floor(index / dim);

        if (shrinking) {
            if (iy > 0)
                --iy;
            else if (++ix == dim)
                break;
        } else {
            if (iy < maxY) {
                if (ix > 0)
                    --ix;
                else
                    ++iy;
            } else {
                ++ix;
            }
        }

        var next = iy * dim + ix;

        var currentIndex = indexes[index];
        var nextIndex = indexes[next];

        if (nextIndex == null)
            break;

        var temp = windowList[currentIndex];
        windowList[currentIndex] = windowList[nextIndex];
        windowList[currentIndex].index = currentIndex;
        windowList[nextIndex] = temp;

        index = next;
    }

    windowList.splice(indexes[index], 1);
    //relayout();
}
