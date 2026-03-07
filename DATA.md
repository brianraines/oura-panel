### Daily Activity:
https://cloud.ouraring.com/v2/docs#operation/Multiple_daily_activity_Documents_v2_usercollection_daily_activity_get
(start_date and end_date of today)
## Display the following values:

- "steps"
- "score"

### Daily Cardiovascular Age:
https://cloud.ouraring.com/v2/docs#operation/Multiple_daily_cardiovascular_age_Documents_v2_usercollection_daily_cardiovascular_age_get
(start_date and end_date of today)
## Display the following values:

- "vascular_age"

### Daily Readiness:
https://cloud.ouraring.com/v2/docs#operation/Multiple_daily_readiness_Documents_v2_usercollection_daily_readiness_get
(start_date and end_date of today)
## Display the following values:

- "score"
- "contributors" -> "hrv_balance"
- "contributors" -> "resting_heart_rate"

### Daily Resilience:
https://cloud.ouraring.com/v2/docs#operation/Multiple_daily_resilience_Documents_v2_usercollection_daily_resilience_get
(start_date and end_date of today)
## Display the following values:

- "level"
- "contributors" -> "stress"

### Daily Sleep:
https://cloud.ouraring.com/v2/docs#operation/Multiple_daily_sleep_Documents_v2_usercollection_daily_sleep_get
(start_date and end_date of last 10 days)
- "score"


### Sleep:
https://cloud.ouraring.com/v2/docs#operation/Multiple_sleep_Documents_v2_usercollection_sleep_get
(start_date and end_date of last 10 days)

## Display the following values:

Of the latest day:
     - "day"
     - "average_breath"
     - "average_heart_rate"
     - "lowest_heart_rate"
     - "average_hrv"
     - "efficiency"
     - "hrv" (heart rate variability) -> "items" -> "value" (highest item)
     - "lowest_heart_rate"
     - "time_in_bed"
     - "awake_time"
     - "bedtime_end"
     - "bedtime_start"
     - "total_sleep_duration"
     - "light_sleep_duration"
     - "rem_sleep_duration"
     - "deep_sleep_duration"

Also calculate the average of the last 10 days of:
- "score" (From Daily Sleep data)
- "time_in_bed"
- "awake_time"
- "total_sleep_duration"
- "light_sleep_duration"
- "rem_sleep_duration"
- "deep_sleep_duration"

Graph the values of the last 10 days with different colors for each:
- "score" (From Daily Sleep data)
- "time_in_bed"
- "awake_time"
- "total_sleep_duration"
- "light_sleep_duration"
- "rem_sleep_duration"
- "deep_sleep_duration"

### Session
URL: https://cloud.ouraring.com/v2/docs#operation/Multiple_session_Documents_v2_usercollection_session_get
(start_date and end_date of today, and use the latest session)

## Display the following values:
- "heart_rate" -> "items" -> "value" (first item)
- "heart_rate_variability" -> "items" -> "value" (first item)
- "mood"
