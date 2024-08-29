#include <include/vdriver/deviceserviceparser.h>

DeviceServiceParser::DeviceServiceParser(TCPClient *client,
                                         Settings::DeviceStruct devSettings,
                                         QObject *parent)
    : m_client(client), m_deviceSettings(devSettings), QObject{parent} {
  m_ConfTimer = new QTimer;
  m_ConfTimer->setInterval(1000);
  connect(m_ConfTimer, &QTimer::timeout, this,
          &DeviceServiceParser::ConfTimerTimeout);
}

void DeviceServiceParser::start(void) {
  QByteArray ba2;
  // ba2.data()[0] ="" '1'";
  ba2.append('0');
  ba2.append('1');

  //  ba2.append(0x0d);
  qsizetype len = ba2.size();
  stepinitport = 0;
  initcurport = 0;
  emit writeToService(ba2);
}

void DeviceServiceParser::newDataReceived(QByteArray &ba) {
  QByteArray ba2, ba3, ba4;
  char b[30], b2[30], np;
  uint baud, databit, parity, stopbit;
  int i;
  qsizetype num, num2;
  bool fl;
  qsizetype len = ba.size();
  qsizetype len2;
  for (i = 0; i < len; i++) {
    b[i] = ba.data()[i];
  }

  switch (stepinitport) {
  case 0:
    if (initcurport != 0) {
      if (ba.data()[0] == 'O' && ba.data()[1] == 'K') {
      } else {
        qWarning() << "  IP  " << m_deviceSettings.ip << " Com Port"
                   << initcurport << " error initialization  ";
        initcurport = 0;
        return;
      }
    }
    initcurport++;
    for (i = initcurport; i < 6; i++) {
      if (m_deviceSettings.ports[i - 1].use) {
        if (checkintport(i)) {
          stepinitport++;
          initcurport = i;
          getportstatus(i);

          //         ba2.append('1');
          //         ba2.append('6');
          //         np=0x30+i;
          //         ba2.append(np);
          /*
              ba2.setNum(16);
              ba3.setNum(i);
              ba2 = ba2 + ba3;
              b[0] = ba2.data()[0];
              b[1] = ba2.data()[1];
              b[2] = ba2.data()[2];

              len2 = ba2.size();

              emit writeToService(ba2);
          */
          return;
        } else
          noerr = false;
      } else
        continue;
    }
    if (noerr)
      isinitDev = true;
    qInfo() << "  IP  " << m_deviceSettings.ip << " configuratin  OK  ";
    break;

  case 1:

    ba2.setNum(m_deviceSettings.ports[initcurport - 1].speed);
    ba2.append(',');
    ba2.append('8');
    ba2.append(',');
    ba2.append(Cpar[m_deviceSettings.ports[initcurport - 1].parity]);
    ba2.append(',');
    ba3.setNum(m_deviceSettings.ports[initcurport - 1].stopbit);
    ba2 = ba2 + ba3;
    //   ba2.append(0x0D);

    len2 = ba2.size();

    for (i = 0; i < len2; i++) {
      b2[i] = ba2.data()[i];
    }

    ba.chop(1);
    //  stopbit = m_deviceSettings.ports[initcurport - 1].stopbit;
    //   parity = m_deviceSettings.ports[initcurport - 1].parity;
    qInfo() << "  IP  " << m_deviceSettings.ip << " Com Port" << initcurport
            << " status  " << ba;

    //        qWarning
    if (ba == ba2) {
      initcurport++;
      for (i = initcurport; i < 6; i++) {
        if (m_deviceSettings.ports[i - 1].use) {
          if (checkintport(initcurport)) {
            //               stepinitport++;
            initcurport = i;
            getportstatus(initcurport);
            return;
          } else
            noerr = false;
        } else
          continue;
      }
      if (noerr) {
        isinitDev = true;
        qInfo() << "  IP  " << m_deviceSettings.ip << " configuratin  OK  ";
      }
      break;
    } else {
      qInfo() << "  IP  " << m_deviceSettings.ip << " Com Port" << initcurport
              << " status in conf file  " << ba2;
      ba3.clear();
      num = ba.indexOf(",");
      ba3 = ba.sliced(num + 1);
      ba4.clear();
      num2 = ba2.indexOf(",");
      ba4 = ba2.sliced(num + 1);
      len = ba3.size();
      len2 = ba4.size();
      for (i = 0; i < len; i++) {
        b[i] = ba3.data()[i];
      }
      for (i = 0; i < len2; i++) {
        b2[i] = ba4.data()[i];
      }

      if (ba3 != ba4)
        isdataformat = true;
      ba3.clear();
      ba3.setNum(m_deviceSettings.ports[initcurport - 1].speed);
      ba4.clear();
      ba4 = ba.first(num);
      len = ba3.size();
      len2 = ba4.size();
      for (i = 0; i < len; i++) {
        b[i] = ba3.data()[i];
      }
      for (i = 0; i < len2; i++) {
        b2[i] = ba4.data()[i];
      }
      if (ba3 != ba4) {
        if (isdataformat)
          stepinitport = 3;
        else {
          stepinitport = 0;
        }
        setbaud(initcurport);
      } else {
        setdataformat(initcurport);
        stepinitport = 0;
      }
    }

    //      if (ba.data()[0] == 'O' && ba.data()[1] == 'K') {
    /*
        num = ba.indexOf(",");
        if (num < 1) {
          initcurport = 0;
          return;
        }

        ba2 = ba.first(num);
        //     ba2=ba.sliced(0,1);
        baud = ba2.toUInt(&fl);
        if (!baud) {
          initcurport = 0;
          return;
        }

        num2 = ba.indexOf(",", num + 1);
        if ((num2 - num) != 2) {
          initcurport = 0;
          return;
        }
        ba2 = ba.sliced(num + 1, 1);

        b[14] = ba2.data()[0];
        len2 = ba2.size();

        stopbit = m_deviceSettings.ports[initcurport].stopbit;
        parity = m_deviceSettings.ports[initcurport].parity;
        ;
        //      }
    */
    break;

  case 3:
    if (ba.data()[0] == 'O' && ba.data()[1] == 'K') {
    } else {
      qWarning() << "  IP  " << m_deviceSettings.ip << " Com Port"
                 << initcurport << " error initialization  ";
      noerr = false;
      initcurport = 0;
      return;
    }

    stepinitport = 0;
    setdataformat(initcurport);

    break;
  };
  /*
    b[0] = ba.data()[0];
    b[1] = ba.data()[1];
    b[2] = ba.data()[2];
    b[3] = ba.data()[3];
    b[4] = ba.data()[4];
    b[5] = ba.data()[5];
    b[6] = ba.data()[6];
*/
  b[7] = ba.data()[7];
}

void DeviceServiceParser::getportstatus(int &np) {
  QByteArray ba2, ba3;

  ba2.setNum(16);
  ba3.setNum(np);
  ba2 = ba2 + ba3;

  emit writeToService(ba2);
}

bool DeviceServiceParser::checkintport(int &np) {
  uint baud, parity, stopbit;
  bool noerr = true;

  baud = m_deviceSettings.ports[np - 1].speed;
  switch (baud) {
  case 1200:
  case 2400:
  case 4800:
  case 9600:
  case 19200:
  case 38400:
  case 57600:
  case 115200:
    break;
  default:
    noerr = false;
    qWarning() << "  IP  " << m_deviceSettings.ip << " Com Port" << np
               << " error baud in conf file  " << baud;
  }

  parity = m_deviceSettings.ports[np - 1].parity;
  if (parity > 2) {
    noerr = false;
    qWarning() << "  IP  " << m_deviceSettings.ip << " Com Port" << np
               << " error parity in conf file  ";
  }

  stopbit = m_deviceSettings.ports[np - 1].stopbit;
  if (stopbit != 1)
    if (!((stopbit == 2) && (parity == 0))) {
      noerr = false;
      qWarning() << "  IP  " << m_deviceSettings.ip << " Com Port" << np
                 << " error stop bity in conf file  " << stopbit;
    }

  return noerr;
}

void DeviceServiceParser::setbaud(int &np) {
  QByteArray ba2 = "06", ba3;

  //  ba2.setNum(0);
  //  ba2.append('6');
  ba3.setNum(np);
  ba2 = ba2 + ba3;

  ba3.setNum(m_deviceSettings.ports[np - 1].speed);
  ba2 = ba2 + ba3;

  qInfo() << "  IP  " << m_deviceSettings.ip << " Com Port" << initcurport
          << " set baud  " << ba3;

  emit writeToService(ba2);
}

void DeviceServiceParser::setdataformat(int &np) {
  QByteArray ba2 = "07", ba3;

  ba3.setNum(np);
  ba2 = ba2 + ba3;

  ba2.append('8');
  ba2.append(Cpar[m_deviceSettings.ports[np - 1].parity]);

  ba3.setNum(m_deviceSettings.ports[np - 1].stopbit);
  ba2 = ba2 + ba3;

  ba3.clear();
  ba3 = ba2.sliced(3, 3);

  qInfo() << "  IP  " << m_deviceSettings.ip << " Com Port" << initcurport
          << " set data format  " << ba3;

  emit writeToService(ba2);
}

void DeviceServiceParser::ConfTimerTimeout() {

  qWarning() << "  IP  " << m_deviceSettings.ip
             << " error time-out configuratin ";
}
