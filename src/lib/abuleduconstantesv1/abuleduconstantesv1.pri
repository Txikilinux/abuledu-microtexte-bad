HEADERS += $$PWD/abuleduconstantesv1.h

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

#note: on recupere le numero de revision du depot bzr principal
#(d'ou l'utilisation de la variable magique $$_PRO_FILE_PWD_
################################################################
#           RECUPERATION BZR REVNO -> GIT pseudo revno
################################################################
#!contains(DEFINES, .*__ABULEDU_CODE_REVNO__.*) {
#  REVNO = $$system(bzr revno $$_PRO_FILE_PWD_)
#  DEFINES += __ABULEDU_CODE_REVNO__=\\\"~revno$${REVNO}\\\"
#}
# 2017 -> passage sur git
!contains(DEFINES, .*__ABULEDU_CODE_REVNO__.*) {
  REVNO = $$system( cd $$_PRO_FILE_PWD_ ; git rev-list HEAD --count )
  DEFINES += __ABULEDU_CODE_REVNO__=\\\"~revno$${REVNO}\\\"
  message(REVNO = $$REVNO)
}


################################################################
#           RECUPERATION BZR BRANCH_NAME (dev/rc/stable)
################################################################
#!contains(DEFINES, .*__ABULEDU_CODE_BRANCHNAME__.*) {
#    BZR_INFO = $$system( bzr info $$_PRO_FILE_PWD_)
#    for(BZR_INFO_BITS, BZR_INFO) {
#        BZR_BRANCH_URL = $${BZR_INFO_BITS}
#    }
#    BZR_BRANCH_NAME = $$dirname(BZR_BRANCH_URL)
#    BZR_BRANCH_NAME = $$basename(BZR_BRANCH_NAME)
#    message(BRANCH_NAME = $$BZR_BRANCH_NAME)
#    DEFINES += __ABULEDU_CODE_BRANCHNAME__=\\\"$$BZR_BRANCH_NAME\\\"
#}
!contains(DEFINES, .*__ABULEDU_CODE_BRANCHNAME__.*) {
    GIT_BRANCH_NAME = $$system( cd $$_PRO_FILE_PWD_ ; git symbolic-ref --short -q HEAD )
    message(BRANCH_NAME = $$GIT_BRANCH_NAME)
    DEFINES += __ABULEDU_CODE_BRANCHNAME__=\\\"$$GIT_BRANCH_NAME\\\"
}
