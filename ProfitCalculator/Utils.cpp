#include "Utils.h"

std::string GetCurrencyName(int currency_, bool short_form_)
{
    std::string temp_string("");

    switch (currency_)
    {
        case ALTERATION_ORB:
        {
            if(short_form_)
                temp_string.append("Alt");
            else
                temp_string.append("Alteration orb");

            break;
        }
        case FUSING_ORB:
        {
            if(short_form_)
                temp_string.append("Fus");
            else
                temp_string.append("Fusing orb");

            break;
        }
        case ALCHEMY_ORB:
        {
            if(short_form_)
                temp_string.append("Alch");
            else
                temp_string.append("Alchemy orb");

            break;
        }
        case CHAOS_ORB:
        {
            if(short_form_)
                temp_string.append("Chaos");
            else
                temp_string.append("Chaos orb");

            break;
        }
        case GEMCUTTER_ORB:
        {
            if(short_form_)
                temp_string.append("GCP");
            else
                temp_string.append("Gemcutter orb");

            break;
        }
        case EXALTED_ORB:
        {
            if(short_form_)
                temp_string.append("Ex");
            else
                temp_string.append("Exalted orb");

            break;
        }
        case CHROMATIC_ORB:
        {
            if(short_form_)
                temp_string.append("Chrom");
            else
                temp_string.append("Chromatic orb");

            break;
        }
        case JEWELLER_ORB:
        {
            if(short_form_)
                temp_string.append("Jew");
            else
                temp_string.append("Jeweller's orb");

            break;
        }
        case CHANCE_ORB:
        {
            if(short_form_)
                temp_string.append("Chance");
            else
                temp_string.append("Chance orb");

            break;
        }
        case CHISEL_ORB:
        {
            if(short_form_)
                temp_string.append("Chis");
            else
                temp_string.append("Chisel orb");

            break;
        }
        case SCOURING_ORB:
        {
            if(short_form_)
                temp_string.append("Scour");
            else
                temp_string.append("Scouring orb");

            break;
        }
        case BLESSED_ORB:
        {
            if(short_form_)
                temp_string.append("Bless");
            else
                temp_string.append("Blessed orb");

            break;
        }
		case REGRET_ORB:
		{
			if (short_form_)
				temp_string.append("Regret");
			else
				temp_string.append("Regret orb");

			break;
		}
        case REGAL_ORB:
        {
            if(short_form_)
                temp_string.append("Regal");
            else
                temp_string.append("Regal orb");

            break;
        }
        case DIVINE_ORB:
        {
            if(short_form_)
                temp_string.append("Divine");
            else
                temp_string.append("Divine orb");

            break;
        }
        case VAAL_ORB:
        {
            if(short_form_)
                temp_string.append("Vaal");
            else
                temp_string.append("Vaal orb");

            break;
        }
        case SCROLL_OF_WISDOM:
        {
            if(short_form_)
                temp_string.append("Wis");
            else
                temp_string.append("Scroll of wisdom");

            break;
        }
        case PORTAL_SCROLL:
        {
            if(short_form_)
                temp_string.append("Port");
            else
                temp_string.append("Portal scroll");

            break;
        }
        case ARMOURERS_SCRAP:
        {
            if(short_form_)
                temp_string.append("Scrap");
            else
                temp_string.append("Armourer's scrap");

            break;
        }
        case WHETSTONE:
        {
            if(short_form_)
                temp_string.append("Whet");
            else
                temp_string.append("Blacksmith's whetstone");

            break;
        }
        case GLASSBLOWERS:
        {
            if(short_form_)
                temp_string.append("GBB");
            else
                temp_string.append("Glassblower's bauble");

            break;
        }
        case TRANSMUTATION_ORB:
        {
            if(short_form_)
                temp_string.append("Transmute");
            else
                temp_string.append("Transmutation orb");

            break;
        }
        case AUGUMENTATION_ORB:
        {
            if(short_form_)
                temp_string.append("Aug");
            else
                temp_string.append("Augumentation orb");

            break;
        }
        case MIRROR_OF_KALANDRA:
        {
            if(short_form_)
                temp_string.append("Mirror");
            else
                temp_string.append("Mirror of Kalandra");

            break;
        }
        case ETERNAL_ORB:
        {
            if(short_form_)
                temp_string.append("Eternal");
            else
                temp_string.append("Eternal orb");

            break;
        }
        case PERANDUS_COIN:
        {
            if(short_form_)
                temp_string.append("P.coin");
            else
                temp_string.append("Perandus coin");


            break;
        }
        case SILVER_COIN:
        {
            if(short_form_)
                temp_string.append("S.coin");
            else
                temp_string.append("Silver coin");

            break;
        }
    }

    return temp_string;
}

std::string GetLeagueName(int league_, bool trade_request_)
{
    std::string temp_string("");

    switch (league_)
    {
        case TEMP_SC:
        {
            if(trade_request_)
                temp_string.append("Legion");
            else
                temp_string.append("Legion SC");

            break;
        }
        case TEMP_HC:
        {
            if(trade_request_)
                temp_string.append("Hardcore+Legion");
            else
                temp_string.append("Legion HC");

            break;
        }
        case STANDARD_SC:
        {
            if(trade_request_)
                temp_string.append("Standard");
            else
                temp_string.append("Standard SC");

            break;
        }
        case STANDARD_HC:
        {
            if(trade_request_)
                temp_string.append("Hardcore");
            else
                temp_string.append("Standard HC");

            break;
        }
    }

    return temp_string;
}
