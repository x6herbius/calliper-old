TEMPLATE = subdirs

SUBDIRS += \
	app-calliper \
    renderer \
    renderer-sandbox \
    model \
    calliperutil \
    app-vmfviewer \
    app-vpkinfo \
    file-formats \
    model-loaders \
    dep-vtflib \
    tst-keyvaluesparser \
    high-level-convenience

renderer-sandbox.depends = renderer calliperutil model
model.depends = renderer calliperutil
renderer.depends = calliperutil
app-vmfviewer.depends = calliperutil renderer model file-formats model-loaders dep-vtflib
app-vpkinfo.depends = file-formats calliperutil
file-formats.depends = calliperutil
model-loaders.depends = model renderer calliperutil file-formats dep-vtflib
dep-qvtf.depends = dep-vtflib
tst-keyvaluesparser.depends = file-formats calliperutil
