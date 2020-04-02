
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "PhysicsTools/TensorFlow/interface/TensorFlow.h"
#include "TLorentzVector.h"

struct ImageTFCache {
  ImageTFCache() : graphDef(nullptr) {}

  std::atomic<tensorflow::GraphDef*> graphDef;
  std::atomic<tensorflow::GraphDef*> graphDefMD;
  //std::atomic<tensorflow::GraphDef*> graphDefPhoMD;
  //std::atomic<tensorflow::GraphDef*> graphDefW;
  std::atomic<tensorflow::GraphDef*> graphDefWMD;
  //std::atomic<tensorflow::GraphDef*> graphDefHbb;
  std::atomic<tensorflow::GraphDef*> graphDefHbbMD;
  //std::atomic<tensorflow::GraphDef*> graphDefHccMD;
  //std::atomic<tensorflow::GraphDef*> graphDefZMD;
  //std::atomic<tensorflow::GraphDef*> graphDefZ;
  //std::atomic<tensorflow::GraphDef*> graphDefWWMD;
  //std::atomic<tensorflow::GraphDef*> graphDefWWlepMD;
  //std::atomic<tensorflow::GraphDef*> graphDefHWWMD;
  //std::atomic<tensorflow::GraphDef*> graphDefHWWlepMD;
};

class ImageProducer : public edm::stream::EDProducer<edm::GlobalCache<ImageTFCache>> {
public:
  explicit ImageProducer(const edm::ParameterSet&, const ImageTFCache*);
  ~ImageProducer() override;
  double runTFlow(tensorflow::Session*, tensorflow::Tensor, tensorflow::Tensor, uint);
  double runTFlow(tensorflow::GraphDef* graphDef, tensorflow::Tensor, tensorflow::Tensor, uint);
  double principalAxis(const std::vector<std::vector<float>>&);
  static void fillDescriptions(edm::ConfigurationDescriptions&);
  static void globalEndJob(const ImageTFCache*);
  static std::unique_ptr<ImageTFCache> initializeGlobalCache(const edm::ParameterSet&);

private:
  void beginStream(edm::StreamID) override {}
  void produce(edm::Event&, const edm::EventSetup&) override;
  void endStream() override {}

  ImageTFCache* cache_;

  tensorflow::Session* tfsession_;
  tensorflow::Session* tfsessionMD_;
  //tensorflow::Session* tfsessionPhoMD_;
  //tensorflow::Session* tfsessionW_;
  tensorflow::Session* tfsessionWMD_;
  //tensorflow::Session* tfsessionHbb_;
  tensorflow::Session* tfsessionHbbMD_;
  //tensorflow::Session* tfsessionHccMD_;
  //tensorflow::Session* tfsessionZMD_;
  //tensorflow::Session* tfsessionZ_;
  //tensorflow::Session* tfsessionWWMD_;
  //tensorflow::Session* tfsessionWWlepMD_;
  //tensorflow::Session* tfsessionHWWMD_;
  //tensorflow::Session* tfsessionHWWlepMD_;

  const edm::EDGetTokenT<edm::View<pat::Jet>> src_;
  const edm::EDGetTokenT<edm::View<pat::Jet>> sj_;

  float minpt_;

  edm::FileInPath pb_path_;
  edm::FileInPath pb_pathMD_;
  //edm::FileInPath pb_pathPhoMD_;
  //edm::FileInPath pb_pathW_;
  edm::FileInPath pb_pathWMD_;
  //edm::FileInPath pb_pathHbb_;
  edm::FileInPath pb_pathHbbMD_;
  //edm::FileInPath pb_pathHccMD_;
  //edm::FileInPath pb_pathZMD_;
  //edm::FileInPath pb_pathZ_;
  //edm::FileInPath pb_pathWWMD_;
  //edm::FileInPath pb_pathWWlepMD_;
  //edm::FileInPath pb_pathHWWMD_;
  //edm::FileInPath pb_pathHWWlepMD_;

  uint nsubs;
};

ImageProducer::ImageProducer(const edm::ParameterSet& iConfig, const ImageTFCache* cache)
    : tfsession_(nullptr),
      tfsessionMD_(nullptr),
      //tfsessionPhoMD_(nullptr),
      //tfsessionW_(nullptr),
      tfsessionWMD_(nullptr),
      //tfsessionHbb_(nullptr),
      tfsessionHbbMD_(nullptr),
      //tfsessionHccMD_(nullptr),
      //tfsessionZMD_(nullptr),
      //tfsessionZ_(nullptr),
      //tfsessionWWMD_(nullptr),
      //tfsessionWWlepMD_(nullptr),
      //tfsessionHWWMD_(nullptr),
      //tfsessionHWWlepMD_(nullptr),
      src_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("src"))),
      sj_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("sj"))),
      minpt_(iConfig.getParameter<double>("minpt")) {
  produces<pat::JetCollection>();

  tensorflow::SessionOptions sessionOptions;
  const ImageTFCache* cache_ = cache;

  tfsession_ = tensorflow::createSession(cache_->graphDef, sessionOptions);
  tfsessionMD_ = tensorflow::createSession(cache_->graphDefMD, sessionOptions);
  //tfsessionPhoMD_ = tensorflow::createSession(cache_->graphDefPhoMD, sessionOptions);
  //tfsessionW_ = tensorflow::createSession(cache_->graphDefW, sessionOptions);
  tfsessionWMD_ = tensorflow::createSession(cache_->graphDefWMD, sessionOptions);
  //tfsessionHbb_ = tensorflow::createSession(cache_->graphDefHbb, sessionOptions);
  tfsessionHbbMD_ = tensorflow::createSession(cache_->graphDefHbbMD, sessionOptions);
  //tfsessionHccMD_ = tensorflow::createSession(cache_->graphDefHccMD, sessionOptions);
  //tfsessionZMD_ = tensorflow::createSession(cache_->graphDefZMD, sessionOptions);
  //tfsessionZ_ = tensorflow::createSession(cache_->graphDefZ, sessionOptions);
  //tfsessionWWMD_ = tensorflow::createSession(cache_->graphDefWWMD, sessionOptions);
  //tfsessionWWlepMD_ = tensorflow::createSession(cache_->graphDefWWlepMD, sessionOptions);
  //tfsessionHWWMD_ = tensorflow::createSession(cache_->graphDefHWWMD, sessionOptions);
  //tfsessionHWWlepMD_ = tensorflow::createSession(cache_->graphDefHWWlepMD, sessionOptions);
}
ImageProducer::~ImageProducer() {
  if (tfsession_ != nullptr)
    tensorflow::closeSession(tfsession_);
  if (tfsessionMD_ != nullptr)
    tensorflow::closeSession(tfsessionMD_);
  //if (tfsessionPhoMD_ != nullptr)
  //  tensorflow::closeSession(tfsessionPhoMD_);
  //if (tfsessionW_ != nullptr)
  //  tensorflow::closeSession(tfsessionW_);
  if (tfsessionWMD_ != nullptr)
    tensorflow::closeSession(tfsessionWMD_);
  //if (tfsessionHbb_ != nullptr)
  //  tensorflow::closeSession(tfsessionHbb_);
  if (tfsessionHbbMD_ != nullptr)
    tensorflow::closeSession(tfsessionHbbMD_);
  //if (tfsessionHccMD_ != nullptr)
  //  tensorflow::closeSession(tfsessionHccMD_);
  //if (tfsessionZMD_ != nullptr)
  //  tensorflow::closeSession(tfsessionZMD_);
  //if (tfsessionZ_ != nullptr)
  //  tensorflow::closeSession(tfsessionZ_);
  //if (tfsessionWWMD_ != nullptr)
  //  tensorflow::closeSession(tfsessionWWMD_);
  //if (tfsessionWWlepMD_ != nullptr)
  //  tensorflow::closeSession(tfsessionWWlepMD_);
  //if (tfsessionHWWMD_ != nullptr)
  //  tensorflow::closeSession(tfsessionHWWMD_);
  //if (tfsessionHWWlepMD_ != nullptr)
  //  tensorflow::closeSession(tfsessionHWWlepMD_);
}

void ImageProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

std::unique_ptr<ImageTFCache> ImageProducer::initializeGlobalCache(const edm::ParameterSet& iConfig) {
  tensorflow::setLogging("3");
  ImageTFCache* cache = new ImageTFCache();

  cache->graphDef = tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_path").fullPath());
  cache->graphDefMD = tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_pathMD").fullPath());
  //cache->graphDefPhoMD =
  //    tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_pathPhoMD").fullPath());
  //cache->graphDefW = tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_pathW").fullPath());
  cache->graphDefWMD =
      tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_pathWMD").fullPath());
  //cache->graphDefHbb =
  //    tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_pathHbb").fullPath());
  cache->graphDefHbbMD =
      tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_pathHbbMD").fullPath());
  //cache->graphDefHccMD =
  //    tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_pathHccMD").fullPath());
  //cache->graphDefZMD =
  //    tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_pathZMD").fullPath());
  //cache->graphDefZ = tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_pathZ").fullPath());
  //cache->graphDefWWMD =
  //    tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_pathWWMD").fullPath());
  //cache->graphDefWWlepMD =
  //    tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_pathWWlepMD").fullPath());
  //cache->graphDefHWWMD =
  //    tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_pathHWWMD").fullPath());
  //cache->graphDefHWWlepMD =
  //    tensorflow::loadGraphDef(iConfig.getUntrackedParameter<edm::FileInPath>("pb_pathHWWlepMD").fullPath());

  return std::unique_ptr<ImageTFCache>(cache);
}

double ImageProducer::runTFlow(tensorflow::Session* tfsession_,
                               tensorflow::Tensor input_image,
                               tensorflow::Tensor input_b,
                               uint NQCDs) {
  std::vector<tensorflow::Tensor> tfoutput;

  tensorflow::Status status =
      tfsession_->Run({{"input_1", input_image}, {"input_2", input_b}}, {"k2tfout_0"}, {}, &tfoutput);

  if (!status.ok()) {
    edm::LogWarning("ImageProducer") << "Tensorflow Failed:" << status.ToString();
    return -1.0;
  }

  float result_top = 0.0;
  float result_qcd = 0.0;

  for (uint i = 0; i < tfoutput[0].flat<float>().size(); ++i) {
    if (tfoutput[0].flat<float>()(i) == 1.0) {
      return 0.0;
    }

    if (i < NQCDs)
      result_qcd += tfoutput[0].flat<float>()(i);
    else
      result_top += tfoutput[0].flat<float>()(i);
  }
  if ((result_top + result_qcd) == 0.0) {
    return 0.0;
  }

  return result_top / (result_top + result_qcd);
}

double ImageProducer::principalAxis(const std::vector<std::vector<float>>& partlist) {
  double tan_theta = 0.0;
  double M11 = 0.0;
  double M20 = 0.0;
  double M02 = 0.0;

  for (uint i = 0; i < partlist[0].size(); ++i) {
    M11 += partlist[0][i] * partlist[1][i] * partlist[2][i];
    M20 += partlist[0][i] * partlist[1][i] * partlist[1][i];
    M02 += partlist[0][i] * partlist[2][i] * partlist[2][i];
  }

  double denom = (M20 - M02 + std::sqrt(4 * M11 * M11 + (M20 - M02) * (M20 - M02)));
  if (denom != 0.0)
    tan_theta = 2.0 * M11 / denom;

  return tan_theta;
}

void ImageProducer::globalEndJob(const ImageTFCache* cache) { delete cache; }

void ImageProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<edm::View<pat::Jet>> jets;
  edm::Handle<edm::View<pat::Jet>> subjets;

  iEvent.getByToken(src_, jets);
  iEvent.getByToken(sj_, subjets);

  TLorentzVector curtlv;
  TLorentzVector sublv;

  int jindex = 0;

  std::vector<float> itopdisc = {};
  std::vector<float> itopdiscMD = {};
  //std::vector<float> itopdiscPhoMD = {};
  //std::vector<float> itopdiscW = {};
  std::vector<float> itopdiscWMD = {};
  //std::vector<float> itopdiscHbb = {};
  std::vector<float> itopdiscHbbMD = {};
  //std::vector<float> itopdiscHccMD = {};
  //std::vector<float> itopdiscZMD = {};
  //std::vector<float> itopdiscZ = {};
  //std::vector<float> itopdiscWWMD = {};
  //std::vector<float> itopdiscWWlepMD = {};
  //std::vector<float> itopdiscHWWMD = {};
  //std::vector<float> itopdiscHWWlepMD = {};

  nsubs = 2;
  int ntopinit = -1;
  for (const auto& AK8pfjet : *jets) {
    ntopinit += 1;

    itopdisc.push_back(-10.0);
    itopdiscMD.push_back(-10.0);
    //itopdiscPhoMD.push_back(-10.0);
    //itopdiscW.push_back(-10.0);
    itopdiscWMD.push_back(-10.0);
    //itopdiscHbb.push_back(-10.0);
    itopdiscHbbMD.push_back(-10.0);
    //itopdiscHccMD.push_back(-10.0);
    //itopdiscZMD.push_back(-10.0);
    //itopdiscZ.push_back(-10.0);
    //itopdiscWWMD.push_back(-10.0);
    //itopdiscWWlepMD.push_back(-10.0);
    //itopdiscHWWMD.push_back(-10.0);
    //itopdiscHWWlepMD.push_back(-10.0);
    if (AK8pfjet.pt() < minpt_)
      continue;
    TLorentzVector curtlv;
    curtlv.SetPtEtaPhiM(AK8pfjet.pt(), AK8pfjet.eta(), AK8pfjet.phi(), AK8pfjet.mass());

    float mergeval = 0.8;

    int ndau = AK8pfjet.numberOfDaughters();
    std::vector<pat::PackedCandidate> allpf;

    std::vector<std::vector<float>> partlist = {{}, {}, {}, {}, {}, {}, {}, {}};
    std::vector<float> sjlist = {};

    double fullint = 0.0;
    double etac = 0;
    double phic = 0;

    int idaufill = 0;
    for (int idau = 0; idau < ndau; idau++) {
      const pat::PackedCandidate* lPack = dynamic_cast<const pat::PackedCandidate*>(AK8pfjet.daughter(idau));
      if ((lPack != nullptr) && (lPack->puppiWeight() > 0)) {
        //For the case that phi loops over
        float dphi = reco::deltaPhi(lPack->phi(), curtlv.Phi());
        TLorentzVector pfclv;
        pfclv.SetPtEtaPhiM(lPack->pt(), lPack->eta(), curtlv.Phi() + dphi, lPack->mass());

        double funcval = (6.62733e-02) + (2.63732e+02) * (1.0 / curtlv.Perp());
        double drcorval = 0.6 / (funcval);

        int pfcflav = std::abs(lPack->pdgId());

        double neweta = pfclv.Eta() + (pfclv.Eta() - curtlv.Eta()) * (drcorval - 1.0);
        double newphi = pfclv.Phi() + (dphi) * (drcorval - 1.0);

        double newdetafj = (pfclv.Eta() - curtlv.Eta()) * drcorval;
        double newdphifj = (dphi)*drcorval;

        if (std::sqrt(newdphifj * newdphifj + newdetafj * newdetafj) > 1.6)
          continue;

        partlist[0].push_back(lPack->pt());
        partlist[1].push_back(neweta);
        partlist[2].push_back(newphi);

        fullint += partlist[0][idaufill];
        etac += partlist[0][idaufill] * partlist[1][idaufill];
        phic += partlist[0][idaufill] * partlist[2][idaufill];

        if (pfcflav == 13)
          partlist[3].push_back(lPack->pt());
        else
          partlist[3].push_back(0.0);
        if (pfcflav == 11)
          partlist[4].push_back(lPack->pt());
        else
          partlist[4].push_back(0.0);
        if (pfcflav == 211)
          partlist[5].push_back(lPack->pt());
        else
          partlist[5].push_back(0.0);
        if (pfcflav == 22)
          partlist[6].push_back(lPack->pt());
        else
          partlist[6].push_back(0.0);
        if (pfcflav == 130)
          partlist[7].push_back(lPack->pt());
        else
          partlist[7].push_back(0.0);
        idaufill += 1;
      }
    }

    for (const auto& subjet : *subjets) {
      sublv.SetPtEtaPhiM(subjet.pt(), subjet.eta(), subjet.phi(), subjet.mass());

      if (sublv.DeltaR(curtlv) > mergeval || sjlist.size() >= (nsubs * 6))
        continue;
      sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probb"));
      sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probbb"));
      sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probuds"));
      sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probg"));
      sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:probc"));
      sjlist.push_back(subjet.bDiscriminator("pfDeepFlavourJetTags:problepb"));
    }

    float gmass = std::max(float(0.0), AK8pfjet.userFloat("ak8PFJetsPuppiSoftDropMass"));
    uint sjlsize = sjlist.size();

    for (uint isj = 0; isj < (nsubs * 6 - sjlsize); isj++)
      sjlist.push_back(0.);

    std::vector<uint> dbindices{uint(sjlist.size() - 1), uint(sjlist.size()) + 2};

    sjlist.push_back(AK8pfjet.bDiscriminator("pfDeepDoubleBvLJetTags:probHbb"));
    sjlist.push_back(AK8pfjet.bDiscriminator("pfDeepDoubleCvLJetTags:probHcc"));
    sjlist.push_back(AK8pfjet.bDiscriminator("pfDeepDoubleCvBJetTags:probHcc"));

    sjlist.push_back(gmass / 172.0);
    sjlist.push_back(AK8pfjet.pt() / 2000.0);

    int npoints = 38;

    std::vector<int> ietalist = {};
    std::vector<int> iphilist = {};

    //Centering and rotating
    etac /= fullint;
    phic /= fullint;
    for (uint i = 0; i < partlist[0].size(); ++i) {
      partlist[1][i] -= etac;
      partlist[2][i] -= phic;
    }

    double tan_theta = principalAxis(partlist);
    double DReta = 1.6;
    double DRphi = 1.6;

    for (uint i = 0; i < partlist[0].size(); ++i) {
      double Reta = partlist[1][i] * std::cos(std::atan(tan_theta)) + partlist[2][i] * std::sin(std::atan(tan_theta));
      double Rphi =
          -1.0 * partlist[1][i] * std::sin(std::atan(tan_theta)) + partlist[2][i] * std::cos(std::atan(tan_theta));

      partlist[1][i] = Reta;
      partlist[2][i] = Rphi;

      ietalist.push_back(int((partlist[1][i] + DReta) / (2.0 * DReta / float(npoints - 1))));
      iphilist.push_back(int((partlist[2][i] + DRphi) / (2.0 * DRphi / float(npoints - 1))));
    }

    uint ncolors = 6;

    std::vector<std::vector<std::vector<double>>> grid(
        37, std::vector<std::vector<double>>(37, std::vector<double>(ncolors, 0.0)));
    std::vector<std::pair<std::vector<uint>, std::vector<double>>> indexedimage;

    //Normalization and digitization
    for (uint i = 0; i < partlist[0].size(); ++i) {
      if ((ietalist[i] >= 37) or (iphilist[i] >= 37) or (ietalist[i] <= 0) or (iphilist[i] <= 0))
        continue;
      int filldex = 0;

      for (uint j = 0; j < partlist.size(); ++j) {
        if (((j > 2) or (j == 0))) {
          grid[ietalist[i]][iphilist[i]][filldex] += partlist[j][i] / fullint;
          filldex += 1;
        }
      }
    }

    for (uint i = 0; i < grid.size(); ++i) {
      for (uint j = 0; j < grid[i].size(); ++j) {
        if (grid[i][j][0] > 0.0000000001) {
          std::pair<std::vector<uint>, std::vector<double>> elem;
          elem.first = {i, j};
          for (uint k = 0; k < grid[i][j].size(); ++k)
            elem.second.push_back(grid[i][j][k]);
          indexedimage.push_back(elem);
        }
      }
    }

    //Flipping horiz and vert
    uint half_img = (npoints - 2) / 2;
    float left_sum = 0.0;
    float right_sum = 0.0;

    for (uint i = 0; i < indexedimage.size(); ++i) {
      if (indexedimage[i].first[0] < half_img)
        left_sum += indexedimage[i].second[0];
      if (indexedimage[i].first[0] > half_img)
        right_sum += indexedimage[i].second[0];
    }
    if (left_sum < right_sum) {
      for (uint i = 0; i < indexedimage.size(); ++i)
        indexedimage[i].first = {npoints - 2 - indexedimage[i].first[0], indexedimage[i].first[1]};
    }

    float lower_sum = 0.0;
    float upper_sum = 0.0;

    for (uint i = 0; i < indexedimage.size(); ++i) {
      if (indexedimage[i].first[1] > half_img)
        lower_sum += indexedimage[i].second[0];
      if (indexedimage[i].first[1] < half_img)
        upper_sum += indexedimage[i].second[0];
    }

    if (lower_sum < upper_sum) {
      for (uint i = 0; i < indexedimage.size(); ++i)
        indexedimage[i].first = {indexedimage[i].first[0], npoints - 2 - indexedimage[i].first[1]};
    }

    tensorflow::Tensor input_b(tensorflow::DT_FLOAT, {1, int(sjlist.size())});
    float* d = input_b.flat<float>().data();

    tensorflow::Tensor input_nodoubleb(tensorflow::DT_FLOAT, {1, int(sjlist.size()) - 3});
    float* dndb = input_nodoubleb.flat<float>().data();

    for (uint i = 0; i < sjlist.size(); ++i, ++d) {
      *d = sjlist[i];
      if ((i <= dbindices[0]) or (i > dbindices[1])) {
        *dndb = sjlist[i];
        ++dndb;
      }
    }

    tensorflow::Tensor input_image(tensorflow::DT_FLOAT, tensorflow::TensorShape({1, 37, 37, ncolors}));
    auto input_map = input_image.tensor<float, 4>();

    for (uint i = 0; i < 37; ++i) {
      for (uint j = 0; j < 37; ++j) {
        for (uint k = 0; k < ncolors; ++k) {
          input_map(0, i, j, k) = 0.0;
        }
      }
    }

    for (uint i = 0; i < indexedimage.size(); ++i) {
      for (uint j = 0; j < indexedimage[i].second.size(); ++j) {
        input_map(0, indexedimage[i].first[0], indexedimage[i].first[1], j) = indexedimage[i].second[j];
      }
    }

    //Actually run tensorflow
    itopdisc[jindex] = runTFlow(tfsession_, input_image, input_nodoubleb, 4);
    itopdiscMD[jindex] = runTFlow(tfsessionMD_, input_image, input_nodoubleb, 4);
    //itopdiscPhoMD[jindex] = runTFlow(tfsessionPhoMD_, input_image, input_b, 4);
    //itopdiscW[jindex] = runTFlow(tfsessionW_, input_image, input_nodoubleb, 4);
    itopdiscWMD[jindex] = runTFlow(tfsessionWMD_, input_image, input_nodoubleb, 4);
    //itopdiscHbb[jindex] = runTFlow(tfsessionHbb_, input_image, input_b, 4);
    itopdiscHbbMD[jindex] = runTFlow(tfsessionHbbMD_, input_image, input_b, 4);
    //itopdiscHccMD[jindex] = runTFlow(tfsessionHccMD_, input_image, input_b, 4);
    //itopdiscZMD[jindex] = runTFlow(tfsessionZMD_, input_image, input_b, 4);
    //itopdiscZ[jindex] = runTFlow(tfsessionZ_, input_image, input_b, 4);
    //itopdiscWWMD[jindex] = runTFlow(tfsessionWWMD_, input_image, input_b, 4);
    //itopdiscWWlepMD[jindex] = runTFlow(tfsessionWWlepMD_, input_image, input_b, 4);
    //itopdiscHWWMD[jindex] = runTFlow(tfsessionHWWMD_, input_image, input_b, 4);
    //itopdiscHWWlepMD[jindex] = runTFlow(tfsessionHWWlepMD_, input_image, input_b, 4);

    jindex += 1;
  }

  //Add to jet userfloats
  auto outputs = std::make_unique<pat::JetCollection>();
  jindex = 0;

  for (const auto& jet : *jets) {
    pat::Jet newJet(jet);

    newJet.addUserFloat("Image:top", itopdisc[jindex]);
    newJet.addUserFloat("ImageMD:top", itopdiscMD[jindex]);
    //newJet.addUserFloat("ImageMD:pho", itopdiscPhoMD[jindex]);
    //newJet.addUserFloat("Image:w", itopdiscW[jindex]);
    newJet.addUserFloat("ImageMD:w", itopdiscWMD[jindex]);
    //newJet.addUserFloat("Image:hbb", itopdiscHbb[jindex]);
    newJet.addUserFloat("ImageMD:hbb", itopdiscHbbMD[jindex]);
    //newJet.addUserFloat("ImageMD:hcc", itopdiscHccMD[jindex]);
    //newJet.addUserFloat("ImageMD:z", itopdiscZMD[jindex]);
    //newJet.addUserFloat("Image:z", itopdiscZ[jindex]);
    //newJet.addUserFloat("ImageMD:ww", itopdiscWWMD[jindex]);
    //newJet.addUserFloat("ImageMD:wwlep", itopdiscWWlepMD[jindex]);
    //newJet.addUserFloat("ImageMD:hww", itopdiscHWWMD[jindex]);
    //newJet.addUserFloat("ImageMD:hwwlep", itopdiscHWWlepMD[jindex]);

    outputs->push_back(newJet);

    jindex += 1;
  }

  //Put into the event
  iEvent.put(std::move(outputs));
}

//Define this as a plug-in
DEFINE_FWK_MODULE(ImageProducer);
