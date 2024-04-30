#!/bin/bash

if [ -d lib ]; then
  rm -rf lib
fi

mkdir lib
bzr checkout --lightweight bzr://redmine.ryxeo.com/code/leterrier-developpeu/abuleduapplicationv1 lib/abuleduapplicationv1
bzr checkout --lightweight bzr://redmine.ryxeo.com/code/leterrier-developpeu/abuleduconstantesv1 lib/abuleduconstantesv1
bzr checkout --lightweight bzr://redmine.ryxeo.com/code/leterrier-developpeu/abuledunetworkaccessmanagerv1/ lib/abuledunetworkaccessmanagerv1
bzr checkout --lightweight bzr://redmine.ryxeo.com/code/leterrier-developpeu/abuleduflatboutonv1 lib/abuleduflatboutonv1
bzr checkout --lightweight bzr://redmine.ryxeo.com/code/leterrier-developpeu/maia/ lib/maia
bzr checkout --lightweight bzr://redmine.ryxeo.com/code/leterrier-developpeu/abuledumessageboxv1/ lib/abuledumessageboxv1
bzr checkout --lightweight bzr://redmine.ryxeo.com/code/leterrier-developpeu/abuleduidentitesv1 lib/abuleduidentitesv1
bzr checkout --lightweight bzr://redmine.ryxeo.com/code/leterrier-developpeu/abuledupluginloaderv1 lib/abuledupluginloaderv1
bzr checkout --lightweight bzr://redmine.ryxeo.com/code/leterrier-developpeu/abuledusingletonv1 lib/abuledusingletonv1
bzr checkout --lightweight bzr://redmine.ryxeo.com/code/leterrier-developpeu/abuleduvirtualkeyboardv1 lib/abuleduvirtualkeyboardv1
bzr checkout --lightweight bzr://redmine.ryxeo.com/code/leterrier-developpeu/abuledufilev1 lib/abuledufilev1
