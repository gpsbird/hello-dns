#pragma once
#include <memory>
#include "dns-storage.hh"
#include "dnsmessages.hh"
#include "comboaddress.hh"

struct AGenerator : RRGenerator
{
  AGenerator(uint32_t ip) : d_ip(ip) {}
  uint32_t d_ip;
  static std::unique_ptr<RRGenerator> make(const ComboAddress&);
  static std::unique_ptr<RRGenerator> make(const std::string& s)
  {
    return make(ComboAddress(s));
  }
  void toMessage(DNSMessageWriter& dpw) override;
};

struct AAAAGenerator : RRGenerator
{
  AAAAGenerator(unsigned char ip[16])
  {
    memcpy(d_ip, ip, 16);
  }
  static std::unique_ptr<RRGenerator> make(const ComboAddress&);
  static std::unique_ptr<RRGenerator> make(const std::string& s)
  {
    return make(ComboAddress(s));
  }
  void toMessage(DNSMessageWriter& dpw) override;
  unsigned char d_ip[16];
};

struct SOAGenerator : RRGenerator
{
  SOAGenerator(const dnsname& mname, const dnsname& rname, uint32_t serial, uint32_t minimum=3600, uint32_t refresh=10800, uint32_t retry=3600, uint32_t expire=604800) :
    d_mname(mname), d_rname(rname), d_serial(serial), d_minimum(minimum), d_refresh(refresh), d_retry(retry), d_expire(expire)
  {}

  template<typename... Targs>
  static std::unique_ptr<RRGenerator> make(Targs&&... fargs)
  {
    return std::move(std::make_unique<SOAGenerator>(std::forward<Targs>(fargs)...));
  }
  void toMessage(DNSMessageWriter& dpw) override;
  dnsname d_mname, d_rname;
  uint32_t d_serial, d_minimum, d_refresh, d_retry, d_expire;
};
