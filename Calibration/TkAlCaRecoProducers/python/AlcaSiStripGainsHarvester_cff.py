import FWCore.ParameterSet.Config as cms

from Calibration.TkAlCaRecoProducers.AlcaSiStripGainsHarvester_cfi import *
from DQMServices.Components.EDMtoMEConverter_cfi import *

EDMtoMEConvertSiStripGains = EDMtoMEConverter.clone()
EDMtoMEConvertSiStripGains.lumiInputTag = cms.InputTag("MEtoEDMConvertSiStripGains","MEtoEDMConverterLumi")
EDMtoMEConvertSiStripGains.runInputTag = cms.InputTag("MEtoEDMConvertSiStripGains","MEtoEDMConverterRun")

DQMStore = cms.Service("DQMStore")

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
dqmEnvSiStripGains = DQMEDAnalyzer('DQMEventInfo',
                                    subSystemFolder = cms.untracked.string('AlCaReco'),  
                                    )

ALCAHARVESTSiStripGains = cms.Sequence( EDMtoMEConvertSiStripGains + alcaSiStripGainsHarvester + dqmEnvSiStripGains )
