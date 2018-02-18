#ifndef _BINFS_OUTPUT_HPP_
#define _BINFS_OUTPUT_HPP_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace BinFS
{

class BinFS
{
private:
  std::vector<std::pair<std::string, std::string>> files;
  std::string hex_to_string(const std::string &in)
  {
    std::string output;
    if ((in.length() % 2) != 0)
    {
      throw std::runtime_error("string is not valid length!");
    }
    size_t cnt = in.length() / 2;
    for (size_t i = 0; cnt > i; ++i)
    {
      uint32_t s = 0;
      std::stringstream ss;
      ss << std::hex << in.substr(i * 2, 2);
      ss >> s;
      output.push_back(static_cast<unsigned char>(s));
    }
    return output;
  }
public:
  BinFS() {};
  ~BinFS() {};
  std::string get_file(const std::string &filename)
  {
    auto it = files.begin();
    for (const std::pair<std::string, std::string> &file : files)
    {
      if (file.first == filename)
      {
        return hex_to_string(file.second);
      }
      it++;
    }
    throw std::runtime_error(filename + " not found!");
  }
  void init()
  {
    files.emplace_back("model_emotion.pb","xÚÜ½mºô8ne;%Qâçp(ŠœÿîZˆt·«œåÌr?İ?n•]ç=q"	ÀpÍ<®y•:ênW+-§ûzËUWMõ©¥özøw®¥İ÷¼«ïm¯¦vÕ’îô=3½~æêç¿üçJüå¹ú¾şé_÷Õ¯ç|ÊïÎ-·ñ»Şu?¼ãšÏ¹fºã.¾zóı©œ'İ‡wŞ­p_¹Vù}ğ™’îr·§å{¦÷Xùäòü}å›×z|óÅ·æ‹ÿ´“ÿú®¿ºŞßÊÍo|[ş½ò«ù7Ÿ‚;iÜI*ùßÁë½êïéªŸ¬¯;Q7Ÿ]ÿüäÿêYî9óSÿyUÆ|j¾›ù[™tıÇ¾üùÏ|İ7¿ÔçÔrwíç½Ò÷®ò­§ŞÏ÷$á{÷µ½íıòN|ìùxÛ5÷ûTÊäg_ùïN}·/¿ßt)Òû=ƒïÏ.ûÌñ<ëÉWŞüıÎï¹òó­÷=OywÎOãå[¯<ÖóıÍş½_«ıú¾ùîw|sÕ—ÏõVúµŸ/çÕs¾»Î½ßç»Zê¼ù;ìÑây®ı®ë½ú=ßU÷µo¾ï™ånÜÇj~¿w¼ïÙ¯—û¼Ç—Såyï{?|¾®rÁ÷´ç<Wç/{¥\üüWÖq]VbÍ|îùŞ=ûù§|×û~-íqóûµJãşúÛïÍu¹±ºë‘¯^Ÿo§2R÷>İŞıãÖ¸ÎÅ>LÖËöiw¶•ø²6xş¯×»pıQ¿4Şç=kµÔØ·ôÉuïÑ[ç6¾vN¹õ½™£úğ+«Ì}”q=ëä6S>ÏjówwıJ­\·ù9ß³ÖšéY™›Dêyînş~Ÿ»¯öò§UöŞìã<\™×ï“ÊÃıî>4yÎ›Uã'»’7ëLß…¿_o»”4ü|#që…ë'îÎõˆ7ß?J.ìë|¯;?›ÛÓç_­óå«–;~/íL~ÿZí£³Î©?|òÆåNóûòÏı}Üß“ËFŞÕ?¤‹×Ÿ7‡ûy8	¹I§ë Ïeñ¾¿yŠÍ‰{‘“õ•çFRÍi³şyŸûX×AO ×_ZİçO}!_!{æa]f×­£9W³Î‰¶Ú¹âu–d&åçfã9	ˆQeO}î†Üçq·‹×Ë.lß³ÎWÏU•Ã]?×…l^7sø„§Nå~Õ›sò¡ßÆñ\íë(Ã,;Gƒ§oyq]ó.›õšœHå)Ÿïìû¿ÚN÷æs¡»gO6Ğwu¾çSNĞg}àü³EüL-Î}_oÎ¬o,,Ï—XE®Í÷ó çoiõ¸°Q‡•øJº^¾Mà±{¾Ú|?ı$å}<x’J
»Vs9“Sfçü§5æfıSnéºıÎ÷;ÆwwÎCšl5ï;<ûÊº¡ôoŸkf6æ{ÖVÿî´Ñ¡ÜZwÖ·^÷¾2ze GùFR<wÜÅTïpb÷{Î÷±/ÜB¿Ïz×SĞ²ÙŸ˜uG».?ò~AŸŞ«s[\o´TÕGgï¢ü°èŸ½T+èÿu¿\·Ş¹!ÿ{ŞÍ÷#;Á¾!„|î{C®²¯¯mÏãáèğ\Ï“‡ç“÷gîë.+/åøÊûÇ?Ü'zŠïAŒt=Êïó¾|¾½³†Ş,¥x÷U=qêa>­<"İè³¯}Xå0Nò|WÔ=z»pÉû[ïa•9[çeÛûò");
  }
};

} // BinFS

#endif // _BINFS_OUTPUT_HPP_

