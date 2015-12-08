#ifndef NMEA_H
#define NMEA_H

#include <QDateTime>
#include <QObject>

#define NMEA_MAXSAT         (12)
#define NMEA_SATINPACK      (4)
#define SUCCESS (1)
#define FAIL  (0)

enum GPSWorkMode {
    UnCertain = 0x00,
    GPS = 0x01,
    BD = 0x02,
    GPSBD = 0x03
};

enum NmeaScentenceType {
    NMEA0183GGA = 0,
    NMEA0183GLL = 1,
    NMEA0183GSA = 2,
    NMEA0183GSV = 3,
    NMEA0183RMC = 4,
    NMEA0183VTG = 5,
    NMEA0183ZDA = 6,
    NMEA0183TXT = 7,
    NMEA0183PMTK = 8
};

/**
 * Date and time data
 * @see nmea_time_now
 */
typedef struct _nmeaTIME
{
    int     year;       /**< Years since 1900 */
    int     mon;        /**< Months since January - [0,11] */
    int     day;        /**< Day of the month - [1,31] */
    int     hour;       /**< Hours since midnight - [0,23] */
    int     min;        /**< Minutes after the hour - [0,59] */
    int     sec;        /**< Seconds after the minute - [0,59] */
    int     hsec;       /**< Hundredth part of second - [0,99] */

} nmeaTIME;

/**
 * Information about satellite
 * @see nmeaSATINFO
 * @see nmeaGPGSV
 */
typedef struct _nmeaSATELLITE
{
    int     id;         /**< Satellite PRN number */
    int     in_use;     /**< Used in position fix */
    int     elv;        /**< Elevation in degrees, 90 maximum */
    int     azimuth;    /**< Azimuth, degrees from true north, 000 to 359 */
    int     sig;        /**< Signal, 00-99 dB */

} nmeaSATELLITE;

/**
 * Information about all satellites in view
 * @see nmeaINFO
 * @see nmeaGPGSV
 */
typedef struct _nmeaSATINFO
{
    int     inuse;      /**< Number of satellites in use (not those in view) */
    int     inview;     /**< Total number of satellites in view */
    nmeaSATELLITE sat[NMEA_MAXSAT]; /**< Satellites information */

} nmeaSATINFO;

/**
 * GGA packet information structure (Global Positioning System Fix Data)
 */
typedef struct _nmeaGPGGA
{
    GPSWorkMode gpsWorkMode;
    QDateTime utc;       /**< UTC of position (just time) */
    double  lat;        /**< Latitude in NDEG - [degree][min].[sec/60] */
    char    ns;         /**< [N]orth or [S]outh */
    double  lon;        /**< Longitude in NDEG - [degree][min].[sec/60] */
    char    ew;         /**< [E]ast or [W]est */
    int     sig;        /**< GPS quality indicator (0 = Invalid; 1 = Fix; 2 = Differential, 3 = Sensitive) */
    int     satinuse;   /**< Number of satellites in use (not those in view) */
    double  HDOP;       /**< Horizontal dilution of precision */
    double  elv;        /**< Antenna altitude above/below mean sea level (geoid) */
    char    elv_units;  /**< [M]eters (Antenna height unit) */
    double  diff;       /**< Geoidal separation (Diff. between WGS-84 earth ellipsoid and mean sea level. '-' = geoid is below WGS-84 ellipsoid) */
    char    diff_units; /**< [M]eters (Units of geoidal separation) */
    double  dgps_age;   /**< Time in seconds since last DGPS update */
    int dgps_sid;   /**< DGPS station ID number */

} nmeaGPGGA;

/**
 * GSA packet information structure (Satellite status)
 */
typedef struct _nmeaGPGSA
{
    GPSWorkMode gpsWorkMode;
    char    fix_mode;   /**< Mode (M = Manual, forced to operate in 2D or 3D; A = Automatic, 3D/2D) */
    int     fix_type;   /**< Type, used for navigation (1 = Fix not available; 2 = 2D; 3 = 3D) */
    int     sat_prn[NMEA_MAXSAT]; /**< PRNs of satellites used in position fix (null for unused fields) */
    double  PDOP;       /**< Dilution of precision */
    double  HDOP;       /**< Horizontal dilution of precision */
    double  VDOP;       /**< Vertical dilution of precision */
} nmeaGPGSA;

/**
 * GSV packet information structure (Satellites in view)
 */
typedef struct _nmeaGPGSV
{
    GPSWorkMode gpsWorkMode;
    int     pack_count; /**< Total number of messages of this type in this cycle */
    int     pack_index; /**< Message number */
    int     sat_count;  /**< Total number of satellites in view */
    nmeaSATELLITE sat_data[NMEA_SATINPACK];

} nmeaGPGSV;

/**
 * RMC packet information structure (Recommended Minimum sentence C)
 */
typedef struct _nmeaGPRMC
{
    GPSWorkMode gpsWorkMode;
    QDateTime utc;       /**< UTC of position */
    char    status;     /**< Status (A = active or V = void) */
    double  lat;        /**< Latitude in NDEG - [degree][min].[sec/60] */
    char    ns;         /**< [N]orth or [S]outh */
    double  lon;        /**< Longitude in NDEG - [degree][min].[sec/60] */
    char    ew;         /**< [E]ast or [W]est */
    double  speed;      /**< Speed over the ground in knots */
    double  direction;  /**< Track angle in degrees True */
    QDate   utc_date;   /**< UTC-DATE of position */
    double  declination; /**< Magnetic variation degrees (Easterly var. subtracts from true course) */
    char    declin_ew;  /**< [E]ast or [W]est */
    char    mode;       /**< Mode indicator of fix type (A = autonomous, D = differential, E = estimated, N = not valid, S = simulator) */

} nmeaGPRMC;

/**
 * VTG packet information structure (Track made good and ground speed)
 */
typedef struct _nmeaGPVTG
{
    GPSWorkMode gpsWorkMode;
    double  dir;        /**< True track made good (degrees) */
    char    dir_t;      /**< Fixed text 'T' indicates that track made good is relative to true north */
    double  dec;        /**< Magnetic track made good */
    char    dec_m;      /**< Fixed text 'M' */
    double  spn;        /**< Ground speed, knots */
    char    spn_n;      /**< Fixed text 'N' indicates that speed over ground is in knots */
    double  spk;        /**< Ground speed, kilometers per hour */
    char    spk_k;      /**< Fixed text 'K' indicates that speed over ground is in kilometers/hour */
    char    pos;        /**< Mode indicator */
} nmeaGPVTG;

/**
  *GLL packet information structure(大地坐标信息)
  */
typedef struct _nmeaGPGLL
{
    GPSWorkMode gpsWorkMode;
    double  lat;        /**< Latitude in NDEG - [degree][min].[sec/60] */
    char    ns;         /**< [N]orth or [S]outh */
    double  lon;        /**< Longitude in NDEG - [degree][min].[sec/60] */
    char    ew;         /**< [E]ast or [W]est */
    QDateTime utc;      /**< UTC of position */
    char    status;     /**< Status A-Data Valid, V-Data Invalid */
} nmeaGPGLL;

/**
  *ZDA packet information structure(UTC时间和日期)
  */
typedef struct _nmeaGPZDA
{
    GPSWorkMode gpsWorkMode;
    QDateTime time;      /**< UTC of position */
    QDate     day;       /**< UTC day of position */
    QDate     month;     /**< UTC month of position */
    QDate     year;      /**< UTC year of position */
    int       ltzh;      /**< Local time zone hours */
    int       ltzn;      /**< Local time zone minutes */
} nmeaGPZDA;

/**
  *TXT packet information structure
  */
typedef struct _nmeaGPTXT
{
    GPSWorkMode gpsWorkMode;
    QString str;
} nmeaGPTXT;

/**
 * Summary GPS information from all parsed packets,
 * used also for generating NMEA stream
 * @see nmea_parse
 * @see nmea_GPGGA2info,  nmea_...2info
 */
typedef struct _nmeaINFO
{
    int     smask;      /**< Mask specifying types of packages from which data have been obtained */

    QDateTime utc;       /**< UTC of position */

    int     sig;        /**< GPS quality indicator (0 = Invalid; 1 = Fix; 2 = Differential, 3 = Sensitive) */
    int     fix;        /**< Operating mode, used for navigation (1 = Fix not available; 2 = 2D; 3 = 3D) */

    double  PDOP;       /**< Position Dilution Of Precision */
    double  HDOP;       /**< Horizontal Dilution Of Precision */
    double  VDOP;       /**< Vertical Dilution Of Precision */

    double  lat;        /**< Latitude in NDEG - +/-[degree][min].[sec/60] */
    double  lon;        /**< Longitude in NDEG - +/-[degree][min].[sec/60] */
    double  elv;        /**< Antenna altitude above/below mean sea level (geoid) in meters */
    double  speed;      /**< Speed over the ground in kilometers/hour */
    double  direction;  /**< Track angle in degrees True */
    double  declination; /**< Magnetic variation degrees (Easterly var. subtracts from true course) */

    nmeaSATINFO satinfo; /**< Satellites information */

} nmeaINFO;

class Nmea : public QObject
{
    Q_OBJECT
public:
    explicit Nmea(QObject *parent = 0);
    ~Nmea();

    bool nmeaParseGpgga(QString *str, nmeaGPGGA *pack);
    int nmeaParseGpgsa(QString *str, nmeaGPGSA *pack);
    int nmeaParseGpgsv(QString *str, nmeaGPGSV *pack);
    int nmeaParseGprmc(QString *str, nmeaGPRMC *pack);
    int nmeaParseGpvtg(QString *str, nmeaGPVTG *pack);
    int nmeaParseGpgll(QString *str, nmeaGPGLL *pack);
    int nmeaParseGpzda(QString *str, nmeaGPZDA *pack);
    int nmeaParseGptxt(QString *str, nmeaGPTXT *pack);

    void nmeaGpgga2info(nmeaGPGGA *pack, nmeaINFO *info);
    void nmeaGpgsa2info(nmeaGPGSA *pack, nmeaINFO *info);
    void nmeaGpgsv2info(nmeaGPGSV *pack, nmeaINFO *info);
    void nmeaGprmc2info(nmeaGPRMC *pack, nmeaINFO *info);
    void nmeaGpvtg2info(nmeaGPVTG *pack, nmeaINFO *info);

signals:
    void beginNextRound();
    void go();

public slots:

private:
    nmeaGPGGA  gpgga;
};

#endif // NMEA_H
