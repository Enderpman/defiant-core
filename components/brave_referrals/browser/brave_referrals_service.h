/* Copyright 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_REFERRALS_BROWSER_BRAVE_REFERRALS_SERVICE_H_
#define BRAVE_COMPONENTS_BRAVE_REFERRALS_BROWSER_BRAVE_REFERRALS_SERVICE_H_

#include <memory>
#include <string>

#include "base/files/file_path.h"
#include "base/macros.h"
#include "base/memory/weak_ptr.h"
#include "base/sequenced_task_runner.h"
#include "base/timer/timer.h"
#include "base/values.h"
#include "url/gurl.h"

class PrefRegistrySimple;
class PrefService;

namespace network {
class SimpleURLLoader;
}

namespace brave {

class BraveReferralsService {
 public:
  explicit BraveReferralsService(PrefService* pref_service);
  ~BraveReferralsService();

  void Start();
  void Stop();

  static bool GetMatchingReferralHeaders(
      const base::ListValue& referral_headers_list,
      const base::DictionaryValue** request_headers_dict,
      const GURL& url);

 private:
  void GetFirstRunTime();
  base::FilePath GetPromoCodeFileName() const;
  void ReadPromoCode();
  void DeletePromoCodeFile() const;
  void MaybeCheckForReferralFinalization();
  void MaybeDeletePromoCodePref() const;
  void InitReferral();
  std::string BuildReferralInitPayload() const;
  std::string BuildReferralFinalizationCheckPayload() const;
  void CheckForReferralFinalization();

  // Invoked from SimpleURLLoader after referral init load
  // completes.
  void OnReferralInitLoadComplete(std::unique_ptr<std::string> response_body);

  // Invoked from SimpleURLLoader after referral finalization check
  // load completes.
  void OnReferralFinalizationCheckLoadComplete(
      std::unique_ptr<std::string> response_body);

  // Invoked after reading contents of promo code file.
  void OnReadPromoCodeComplete();

  bool initialized_;
  base::Time first_run_timestamp_;
  scoped_refptr<base::SequencedTaskRunner> task_runner_;
  std::unique_ptr<network::SimpleURLLoader> referral_init_loader_;
  std::unique_ptr<network::SimpleURLLoader> referral_finalization_check_loader_;
  PrefService* pref_service_;
  std::string promo_code_;

  base::WeakPtrFactory<BraveReferralsService> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(BraveReferralsService);
};

// Creates the BraveReferralsService
std::unique_ptr<BraveReferralsService> BraveReferralsServiceFactory(
    PrefService* pref_service);

// Registers the preferences used by BraveReferralsService
void RegisterPrefsForBraveReferralsService(PrefRegistrySimple* registry);

}  // namespace brave

#endif  // BRAVE_COMPONENTS_BRAVE_REFERRALS_BROWSER_BRAVE_REFERRALS_SERVICE_H_
