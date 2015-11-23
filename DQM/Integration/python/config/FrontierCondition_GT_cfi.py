import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.FrontierConditions_GlobalTag_cff import * 
GlobalTag.connect = cms.string("frontier://(proxyurl=http://localhost:3128)(serverurl=http://localhost:8000/FrontierOnProd)(serverurl=http://localhost:8000/FrontierOnProd)(retrieve-ziplevel=0)(failovertoserver=no)/CMS_CONDITIONS")
GlobalTag.globaltag = "75X_dataRun2_HLTHI_v4"
es_prefer_GlobalTag = cms.ESPrefer('PoolDBESSource','GlobalTag')
