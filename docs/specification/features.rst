Features
=========

NDEF message
--------------

.. feat:: NDEF message type
   :id: CODEC_FEAT_1
   :links: CODEC_SPEC_1

   The message type is 0x03, corresponding to a known type.

.. feat:: NDEF message length
   :id: CODEC_FEAT_2
   :links: CODEC_SPEC_1

   Message length in bytes as a 16-bit value.

   Byte 2 is unused so 0xFF.
   Byte 1 holds the Most Significant 8-bits.
   Byte 0 holds the Least Significant 8 bits.

   There is no function to change this after the message has been created.

NDEF record
^^^^^^^^^^^^

.. feat:: Payload length
   :id: CODEC_FEAT_3
   :status: complete
   :links: CODEC_SPEC_3

   Length of the NDEF record payload length in bytes. Similar to :need:`CODEC_FEAT_2`,
   it cannot change after the record has been created.

.. feat:: Type length
   :id: CODEC_FEAT_4
   :status: complete
   :links: CODEC_SPEC_3

   Length of the :need:`CODEC_FEAT_5` field in bytes. This is 1 byte.

.. feat:: Record type
   :id: CODEC_FEAT_5
   :status: complete
   :links: CODEC_SPEC_3

   NDEF record type is 0x55, which corresponds to a URI record.

URL Parameters
~~~~~~~~~~~~~~~
.. feat:: Sample interval b64
   :id: CODEC_FEAT_10
   :status: complete
   :links: CODEC_SPEC_3

   The time interval between samples in minutes. This must be constant.

.. feat:: Serial
   :id: CODEC_FEAT_38
   :status: complete
   :links: CODEC_SPEC_3

   An 8 character serial string uniquely identifies the encoder instance. More generally this will
   identify the hardware that the encoder is running on. Characters from the base64 dictionary are
   recommended for these are URL safe.

.. feat:: CodecVersion
   :id: CODEC_FEAT_41
   :status: open
   :links: CODEC_SPEC_18, CODEC_FEAT_43

   16-bit unsigned integer codec version. From this the decoder can raise an error
   if it is not compatible.

.. feat:: FormatCode
   :id: CODEC_FEAT_42
   :status: open
   :links: CODEC_SPEC_18

   8-bit identifier of the circular buffer format. The circular buffer is arranged into pairs.
   A sample either corresponds to a pair of readings (e.g. temperature and humidity), or a single reading
   (temperature only). The latter option doubles the number of samples in the buffer.

   The device is specified as HDC2021. This allows the decoder to convert from the sensor ADC value (a 12-bit integer)
   into floating point degrees C or percent. `Equations <https://www.ti.com/document-viewer/HDC2021/datasheet/address-0x02-humidity-lsb-snas6782213#SNAS6782213>`_
   to do this are device specific.

   +------------+-------------------------------------+
   | FormatCode | Definition                          |
   +------------+-------------------------------------+
   | 0          | HDC2021_TRH_OnePairPerSample        |
   +------------+-------------------------------------+
   | 1          | HDC2021_T_OneReadingPerSample       |
   +------------+-------------------------------------+

.. feat:: Error raised if versions mismatch
   :id: CODEC_FEAT_43
   :status: open
   :links: CODEC_SPEC_19

   If the decoder version does not match that of the encoder used to produce the URL, then :need:`CODEC_REQ_2`
   cannot be guaranteed.

.. feat:: Protocol
   :id: CODEC_FEAT_44
   :status: open
   :links: CODEC_SPEC_3

   The HTTPS protocol is recommended for production use.

   +----------+-------------+
   | Protocol | Definition  |
   +----------+-------------+
   | 0x03     | http://     |
   +----------+-------------+
   | 0x04     | https://    |
   +----------+-------------+

Status
~~~~~~~~

.. feat:: LoopCount
   :id: CODEC_FEAT_28
   :status: complete
   :links: CODEC_SPEC_15

   The number of times the circular buffer has looped from the last EEPROM block to
   the first since initialisation. See :cpp:member:`loopcount`.

.. feat:: ResetsAllTime
   :id: CODEC_FEAT_29
   :status: complete
   :links: CODEC_SPEC_15

   Number of times the microcontroller running the encoder has reset. Each reset causes a counter to be incremented in
   non-volatile memory (:cpp:member:`resetsalltime`).

.. feat:: BatV
   :id: CODEC_FEAT_30
   :status: complete
   :links: CODEC_SPEC_15

   The battery voltage in mV. See :cpp:member:`batvoltage`.

ResetCause
"""""""""""

.. feat:: BOR
   :id: CODEC_FEAT_31
   :status: complete
   :tags: bit
   :links: CODEC_SPEC_16

   Brown Out Reset flag.

.. feat:: SVSH
   :id: CODEC_FEAT_32
   :status: complete
   :tags: bit
   :links: CODEC_SPEC_16

   Supply Voltage Supervisor error flag.

.. feat:: WDT
   :id: CODEC_FEAT_33
   :status: complete
   :tags: bit
   :links: CODEC_SPEC_16

   Watchdog Timeout flag

.. feat:: MISC
   :id: CODEC_FEAT_34
   :status: complete
   :tags: bit
   :links: CODEC_SPEC_16

   Miscellaneous Error flag

.. feat:: LPM5WU
   :id: CODEC_FEAT_35
   :status: complete
   :tags: bit
   :links: CODEC_SPEC_16

   Low Power Mode x.5 wakeup flag.

.. feat:: CLOCKFAIL
   :id: CODEC_FEAT_36
   :status: complete
   :tags: bit
   :links: CODEC_SPEC_16

   Clock failure flag.

.. feat:: SCANTIMEOUT
   :id: CODEC_FEAT_37
   :status: complete
   :tags: bit
   :links: CODEC_SPEC_16

   Scan timeout flag.


Circular Buffer
~~~~~~~~~~~~~~~~~

.. feat:: Error raised if hash check fails
   :id: CODEC_FEAT_40
   :status: complete
   :links: CODEC_SPEC_10

   The decoder independently calculates the hash of the circular buffer and compares it with
   the one contained in :need:`CODEC_SPEC_13`. If the check fails then no samples are returned
   and an exception is raised.

   If the MD5 hash is used then this indicates the decoded sample list does not correspond to that
   fed into the encoder. Therefore :need:`CODEC_REQ_2` has not been met.

   If the HMAC hash is used then there is an additional possibility: authentication has failed.
   The secret key used by the encoder and the stored copy used by the decoder do not match. This occurs
   when the software is run by an unauthorised 3rd party.

.. feat:: Adjustable buffer length.
   :id: CODEC_FEAT_23
   :status: complete
   :links: CODEC_SPEC_12

   The length of the circular buffer can be adjusted. This is done with a compiler parameter,
   to meet :need:`CODEC_FEAT_8`.

.. feat:: Hash
   :id: CODEC_FEAT_24
   :status: complete
   :links: CODEC_SPEC_14

   The list of samples in the buffer must always be transmitted together with a hash. This is used
   by the decoder to verify that it has unwrapped the circular buffer and decoded samples correctly.

   The size of the URL is limited, so there is only room to store the least significant 7 bytes of the hash,
   however, this should be ample. The hash does not contain :need:`CODEC_FEAT_26` and therefore it does not
   need to be recalculated each time this changes. This is done in order to save power :need:`CODEC_SPEC_8`.

   If Hash Based Message Authentication (HMAC) is enabled, then the last characters of the HMAC-MD5 will
   be used. If not, these will be the output of MD5 only.

   The hash is used as a checksum; a guard against unintentional data corruption. This may arise
   because of a bug in the codec. The MD5 is sufficient for this purpose. It was chosen in order to adhere
   with :need:`CODEC_SPEC_4`. The MD5 hash alone is useless for security purposes. If a bad actor
   intends to find a collision (i.e. two sets of data that produce the same MD5 hash) then this can be done
   with ease. MD5 is intended for debug and code development only.

   HMAC-MD5 is considerably more secure than MD5 alone. It is recommended for production use. Each device with an encoder
   should have a unique secret key. In addition to data integrity, HMAC-MD5 can be used to verify that the
   decoder and encoder have access to the same shared secret key. It is therefore a check on authenticity.

    From `Wikipedia <https://en.wikipedia.org/wiki/HMAC>`_:
        The cryptographic strength of the HMAC depends upon the size of the secret key that is used.
        The most common attack against HMACs is brute force to uncover the secret key.
        HMACs are substantially less affected by collisions than their underlying hashing algorithms alone.[6][7]
        In particular, in 2006 Mihir Bellare proved that HMAC is a PRF under the sole assumption
        that the compression function is a PRF.[8] Therefore, HMAC-MD5 does not suffer from the same weaknesses
        that have been found in MD5.

        ...

        For HMAC-MD5 the RFC summarizes that – although the security of the MD5 hash function itself is
        severely compromised – the currently known "attacks on HMAC-MD5 do not seem to indicate a practical
        vulnerability when used as a message authentication code", but it also adds that
        "for a new protocol design, a ciphersuite with HMAC-MD5 should not be included".

   It is acknowledged that HMAC-MD5 has been used despite the counter-recommendation above. I
   decided that the increased complexity of HMAC-SHA3 cannot be justified: The algorithm has to run
   with low energy consumption on an inexpensive microcontroller (see :need:`CODEC_REQ_12`).
   The MSP430 itself is not designed for a high degree of data security. De-lidding and X-raying are possible.
   This is why it is important not to share the secret key between devices.
   Opting for a more robust hashing algorithm may result in compromises elsewhere (e.g. on battery life). It is also the case that environmental sensor
   data are being transmitted and not data that are highly sensitive. The reward to compromise this system
   is sufficiently low to make HMAC-MD5 a good-enough deterrent.

   Data are hashed in the following order:

    +------+-----------------------+--------------+
    | Byte | Field                 | Value        |
    +======+=======================+==============+
    | 0    | Pair[0]               | Reading0_MSB |
    +------+                       +--------------+
    | 1    |                       | Reading1_MSB |
    +------+                       +--------------+
    | 2    |                       | LSB          |
    +------+-----------------------+--------------+
    | 3    | Pair[1]               | Reading0_MSB |
    +------+                       +--------------+
    | 4    |                       | Reading1_MSB |
    +------+                       +--------------+
    | 5    |                       | LSB          |
    +------+-----------------------+--------------+
    | ...  | ...                   |              |
    +------+-----------------------+--------------+
    | L-11 | Pair[NPairs-1]        | Reading0_MSB |
    +------+                       +--------------+
    | L-10 |                       | Reading1_MSB |
    +------+                       +--------------+
    | L-9  |                       | LSB          |
    +------+-----------------------+--------------+
    | L-8  | :need:`CODEC_FEAT_28` | MSB          |
    +------+                       +--------------+
    | L-7  |                       | LSB          |
    +------+-----------------------+--------------+
    | L-6  | :need:`CODEC_FEAT_29` | MSB          |
    +------+                       +--------------+
    | L-5  |                       | LSB          |
    +------+-----------------------+--------------+
    | L-4  | :need:`CODEC_FEAT_30`                |
    +------+--------------------------------------+
    | L-3  | :need:`CODEC_SPEC_16`                |
    +------+-----------------------+--------------+
    | L-2  | endstopindex          | MSB          |
    +------+                       +--------------+
    | L-1  |                       | LSB          |
    +------+-----------------------+--------------+

.. feat:: NPairs
   :id: CODEC_FEAT_25
   :status: complete
   :links: CODEC_SPEC_14

   Number of valid samples in the circular buffer. This excludes samples used for padding.
   Populated from :cpp:var:`npairs`.

.. feat:: Elapsed b64
   :id: CODEC_FEAT_26
   :status: complete
   :links: CODEC_SPEC_13

   External to the codec is a counter. This increases by 1 every minute after the previous
   sample was written to the circular buffer. It resets to 0 when a new sample is written.

   The decoder uses it to determine to the nearest minute when samples were collected. Without it,
   the maximum resolution on the timestamp for each sample would be equal to the time interval, which
   can be up to 60 minutes.

   The unencoded minutes elapsed field is 16-bits wide. This is the same width
   as the unencoded time interval in minutes field.

   The minutes elapsed field occupies 4 bytes after base64 encoding, including one
   padding byte. By convention this is 0x61 or '='.

   The encoder replaces the padding byte with :c:macro:`ENDSTOP_BYTE`. This marks the last byte of the end stop.

   The first step performed by the decoder is to locate :c:macro:`ENDSTOP_BYTE`. After it is
   found, it can be replaced with an '=' before the minutes elapsed field is
   decoded from base64 into its original 16-bit value.

Flags + TNF
~~~~~~~~~~~~

.. feat:: MB
   :id: CODEC_FEAT_17
   :status: complete
   :tags: bit
   :links: CODEC_SPEC_5

   Message Begin bit denotes the first record in an NDEF message.

   This is set. The record is the first in the message.

.. feat:: ME
   :id: CODEC_FEAT_18
   :status: complete
   :tags: bit
   :links: CODEC_SPEC_5

   Message End bit denotes the last record in an NDEF message.

   This is set. The record is the last in the message.

.. feat:: CF
   :id: CODEC_FEAT_19
   :status: complete
   :tags: bit
   :links: CODEC_SPEC_5

   Chunk Flag bit denotes a message comprised of several records chunked together (concatenated).

   This is cleared. There is only one record in the message.

.. feat:: SR
   :id: CODEC_FEAT_20
   :status: complete
   :tags: bit
   :links: CODEC_SPEC_5

   Short Record bit. When set :need:`CODEC_FEAT_3` one byte long. When cleared it is 4 bytes long.

   This is cleared, because the message is longer than 255 bytes.

.. feat:: IL
   :id: CODEC_FEAT_21
   :status: complete
   :tags: bit
   :links: CODEC_SPEC_5

   ID Length bit. When set the ID length field is present. When cleared it is omitted.

   This is cleared.

.. feat:: TNF
   :id: CODEC_FEAT_22
   :status: complete
   :tags: bit
   :links: CODEC_SPEC_5

   Type Name Format field. A 3-bit value that describes the record type.

   This is set to 0x03, which corresponds to an Absolute URI Record.

Other
------

.. feat:: No absolute timestamp
   :id: CODEC_FEAT_27
   :links: CODEC_SPEC_6, CODEC_SPEC_10

   The URL from the encoder cannot include an absolute timestamp. This would
   need to be set each time the microcontroller is powered on (e.g. when the battery is replaced).

.. feat:: Samples timestamped by decoder
   :id: CODEC_FEAT_6
   :links: CODEC_SPEC_10

   All samples are timestamped relative to the time that the decoder is run. It
   is assumed that the time difference between when the encoded message is read (by a phone) and
   the time the decoder is run (on a web server) is much less than one minute. Timestamp precision is one
   minute.

   The timestamping algorithm is as follows:
   #. Samples are put in order of recency.
   #. Minutes :need:`CODEC_FEAT_26` since the most recent sample is extracted from the URL.
   #. Current time (now in UTC) is determined.
   #. The first sample is assigned a timestamp = now - minutes elapsed.
   #. :need:`CODEC_FEAT_10` between samples is extracted from the URL. This is used to timestamp each sample
   relative to the first.

.. feat:: Base URL
   :id: CODEC_FEAT_7
   :links: CODEC_SPEC_3

   The base URL can be changed. It is recommended to keep this as short as possible to
   allow more room for environmental sensor data.

Low resource utilisation
-----------------------
.. feat:: Encoder writes to EEPROM blocks.
   :id: CODEC_FEAT_13
   :status: complete
   :links: CODEC_SPEC_4

   The encoder cannot output the 1000 character NDEF message in one go. This would require
   too much RAM for a small microcontroller.

   Instead it is designed to output an I2C EEPROM, which is arranged into
   16-byte blocks. A maximum of 4 EEPROM blocks are written to or read from at a time.

.. feat:: Only static memory allocation is used.
   :id: CODEC_FEAT_8
   :status: complete
   :links: CODEC_SPEC_4

   The stdio library needed for malloc takes a lot of available memory on the MSP430, so it is not used.

.. feat:: Encoder is written in C.
   :id: CODEC_FEAT_9
   :status: complete
   :links: CODEC_SPEC_4

   There is little benefit to C++ given the low complexity of the encoder.

.. feat:: No RTOS is required
   :id: CODEC_FEAT_14
   :status: complete
   :links: CODEC_SPEC_8, CODEC_SPEC_4

   An RTOS is not appropriate for this application. It will significantly increase the memory footprint.
   It will add complexity and make power consumption more difficult to control.

.. feat:: Status updates once per loop
   :id: CODEC_FEAT_39
   :status: complete
   :links: CODEC_SPEC_15

   Status contains some parameters that change infrequently. For these, :need:`CODEC_SPEC_2` is not a
   concern. :need:`CODEC_FEAT_28` and :need:`CODEC_FEAT_30` are updated once, when cursorblk and nextblk
   are at opposite ends of the circular buffer (e.g. cursorblk == 31 and nextblk == 0). This will
   happen once per day.

   :need:`CODEC_SPEC_16` is cleared when this happens, because a reset has not occurred recently.

.. feat:: Full message written on startup.
   :id: CODEC_FEAT_12
   :status: complete
   :links: CODEC_SPEC_1

   The entire NDEF message only needs to be written once upon startup. Afterwards, small
   parts of the message are modified at a time.

.. feat:: Append sample.
   :id: CODEC_FEAT_15
   :status: complete
   :links: CODEC_SPEC_12

   The list of environmental sensor readings (and its HMAC) will change at an interval of
   time interval minutes. If the time interval is set to 5 minutes, 100K writes will be
   reached in (5 minutes * 100e3) = 1 year.

   By using a circular buffer, these writes are distributed across many blocks. This is
   a form of `Wear levelling <https://en.wikipedia.org/wiki/Wear_leveling>`.

.. feat:: The encoder writes two circular buffer blocks at a time.
   :id: CODEC_FEAT_16
   :status: complete
   :links: CODEC_SPEC_4, CODEC_SPEC_2, CODEC_SPEC_8

   This reduces the requirement for RAM on the MSP430 and reduces power consumption (it takes time to write
   EEPROM blocks).
